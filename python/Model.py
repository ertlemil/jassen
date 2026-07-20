import time

import torchrl.data
from matplotlib.patches import StepPatch
from tensordict.nn import NormalParamExtractor, TensorDictModule
from torch.distributions import Categorical

from Env import JassEnv
import warnings
warnings.filterwarnings("ignore")
from torch import multiprocessing


from collections import defaultdict

import torch
from torch import nn
from torchrl.collectors import Collector
from torchrl.data.replay_buffers import ReplayBuffer
from torchrl.data.replay_buffers.samplers import SamplerWithoutReplacement
from torchrl.data.replay_buffers.storages import LazyTensorStorage
from torchrl.envs import (StepCounter, TransformedEnv)
from torchrl.envs.utils import ExplorationType, set_exploration_type
from torchrl.modules import ProbabilisticActor, MaskedCategorical
from torchrl.objectives import ClipPPOLoss
from torchrl.objectives.value import GAE
from tqdm import tqdm


env = TransformedEnv(JassEnv(), StepCounter())
eval_env = TransformedEnv(JassEnv(), StepCounter())


is_fork = multiprocessing.get_start_method() == "fork"
device = (
    torch.device(0)
    if torch.cuda.is_available() and not is_fork
    else torch.device("cpu")
)


lr = 2e-4
max_grad_norm = 1.0
frames_per_batch = 4096
total_frames = frames_per_batch * 200
sub_batch_size = 128
num_epochs = 3
clip_epsilon = (
    0.2
)
gamma = 1
lmbda = 0.95
entropy_eps = 1e-2

actor_net = nn.Sequential(
    nn.Linear(350, 256, device=device),
    nn.LayerNorm(256, device=device),
    nn.Tanh(),
    nn.Linear(256, 128, device=device),
    nn.Tanh(),
    nn.Linear(128, 36, device=device),
)

critic_net = nn.Sequential(
    nn.Linear(410, 256, device=device),
    nn.LayerNorm(256, device=device),
    nn.Tanh(),
    nn.Linear(256, 128, device=device),
    nn.Tanh(),
    nn.Linear(128, 1, device=device)
)

class Buffer:
    def __init__(self):
        self.states = []
        self.actions = []
        self.log_probs = []
        self.mask = []
        self.rewards = []

        self.critic_states = []
        self.predictions = []


policy_module = TensorDictModule(
    actor_net, in_keys=["observation"], out_keys=["logits"]
)

policy = ProbabilisticActor(
    module=policy_module,
    spec=env.action_spec,
    in_keys={"logits": "logits", "mask": "action_mask"},
    distribution_class=MaskedCategorical,
    return_log_prob=True,
    out_keys=["action"],
)

critic_module = TensorDictModule(
    critic_net, in_keys=["observation_critic"], out_keys=["state_value"]
)

collector = Collector(
    env,
    policy,
    frames_per_batch=frames_per_batch,
    total_frames=total_frames,
    split_trajs=False,
    device=device,
)

replay_buffer = ReplayBuffer(
    storage=LazyTensorStorage(max_size=frames_per_batch),
    sampler=SamplerWithoutReplacement(),
)

advantage_module = GAE(
    gamma=gamma, lmbda=lmbda, value_network=critic_module, average_gae=True, device=device,
)

loss_module = ClipPPOLoss(
    actor_network=policy,
    critic_network=critic_module,
    clip_epsilon=clip_epsilon,
    entropy_bonus=bool(entropy_eps),
    entropy_coeff=entropy_eps,
)

optim = torch.optim.Adam(loss_module.parameters(), lr)
scheduler = torch.optim.lr_scheduler.CosineAnnealingLR(
    optim, total_frames // frames_per_batch, 0.0
)

start = time.perf_counter()

for i, tensordict_data in enumerate(collector):
    if i == 100000:
        break
end = time.perf_counter()
print(end - start)

"""
logs = defaultdict(list)
pbar = tqdm(total=total_frames)
eval_str = ""

for i, tensordict_data in enumerate(collector):
    for _ in range(num_epochs):
        advantage_module(tensordict_data)
        data_view = tensordict_data.reshape(-1)
        replay_buffer.extend(data_view.cpu())
        for _ in range(frames_per_batch // sub_batch_size):
            subdata = replay_buffer.sample(sub_batch_size)
            loss_vals = loss_module(subdata.to(device))
            loss_value = (
                    loss_vals["loss_objective"]
                    + loss_vals["loss_critic"]
                    + loss_vals["loss_entropy"]
            )

            loss_value.backward()
            torch.nn.utils.clip_grad_norm_(loss_module.parameters(), max_grad_norm)
            optim.step()
            optim.zero_grad()

    logs["reward"].append(tensordict_data["next", "reward"].mean().item())
    pbar.update(tensordict_data.numel())
    cum_reward_str = (
        f"average reward={logs['reward'][-1]: 4.4f} (init={logs['reward'][0]: 4.4f})"
    )
    logs["step_count"].append(tensordict_data["step_count"].max().item())
    stepcount_str = f"step count (max): {logs['step_count'][-1]}"
    logs["lr"].append(optim.param_groups[0]["lr"])
    lr_str = f"lr policy: {logs['lr'][-1]: 4.4f}"

    if i % 10 == 0:
        with set_exploration_type(ExplorationType.DETERMINISTIC), torch.no_grad():
            eval_rollout = eval_env.rollout(1000, policy)
            logs["eval reward"].append(eval_rollout["next", "reward"].mean().item())
            logs["eval reward (sum)"].append(
            eval_rollout["next", "reward"].sum().item()
        )
        logs["eval step_count"].append(eval_rollout["step_count"].max().item())
        eval_str = (
        f"eval cumulative reward: {logs['eval reward (sum)'][-1]: 4.4f} "
        f"(init: {logs['eval reward (sum)'][0]: 4.4f}), "
        f"eval step-count: {logs['eval step_count'][-1]}"
        )
        del eval_rollout

    pbar.set_description(", ".join([eval_str, cum_reward_str, stepcount_str, lr_str]))

    # We're also using a learning rate scheduler. Like the gradient clipping,
    # this is a nice-to-have but nothing necessary for PPO to work.
    scheduler.step()

pbar.close()
collector.shutdown()
"""