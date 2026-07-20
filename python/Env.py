import torch
from tensordict import TensorDict, TensorDictBase

from torchrl.data import Composite, UnboundedContinuous, Binary, Categorical
from torchrl.envs import EnvBase

import jass_engine


class JassEnv(EnvBase):
    def __init__(self):
        super().__init__()
        self.engine = jass_engine.Game()

        #Change for Multithreading
        self.batch_size = torch.Size([])

        self._make_specs()

    def _step(self, tensordict):
        action = int(tensordict["action"].item())
        result = self.engine.step(action)
        return TensorDict(
            {
                "observation": torch.tensor(result.nextObservation, dtype=torch.float32),
                "observation_critic": torch.tensor(result.nextCriticObservation, dtype=torch.float32),
                "action_mask": torch.tensor(result.mask, dtype=torch.bool),
                "reward": torch.tensor(result.reward, dtype=torch.float32),
                "terminated": torch.tensor([result.done], dtype=torch.bool),
            },
        batch_size=tensordict.shape).to(device=tensordict.device)

    def _reset(self, tensordict):
        if tensordict is None or tensordict.is_empty():
            tensordict = self.gen_params()

        result = self.engine.reset()
        return TensorDict(
            {
                "observation": torch.tensor(result.nextObservation, dtype=torch.float32),
                "observation_critic": torch.tensor(result.nextCriticObservation, dtype=torch.float32),
                "action_mask": torch.tensor(result.mask, dtype=torch.bool),
                "terminated": torch.tensor([result.done], dtype=torch.bool),
            },
        batch_size=tensordict.shape).to(tensordict.device)

    def _set_seed(self, seed):
        pass

    def _make_specs(self):
        self.observation_spec = Composite( 
            observation=UnboundedContinuous(
                shape=torch.Size([*self.batch_size, 350]),
                dtype=torch.float32
            ),
            observation_critic=UnboundedContinuous(
                shape=torch.Size([*self.batch_size, 410]),
                dtype=torch.float32
            ),
            action_mask=Binary(
                shape=torch.Size([*self.batch_size, 36]),
                dtype=torch.bool
            ),
            shape=self.batch_size
        )

        self.action_spec = Categorical(
            n=36,
            shape=torch.Size([*self.batch_size]),
            dtype=torch.int64
        )

        self.reward_spec = UnboundedContinuous(shape=torch.Size([*self.batch_size, 1]), dtype=torch.float32)

        self.done_spec = Binary(shape=torch.Size([*self.batch_size, 1]), dtype=torch.bool)

    def gen_params(self) -> TensorDictBase:
        td = TensorDict(
            {
                "observation": torch.zeros((1, 350), dtype=torch.float32), #350x float32
                "observation_critic": torch.zeros((1, 410), dtype=torch.float32), #410x float32
                "action_mask": torch.zeros((1, 36), dtype=torch.bool), #36x bool
                "reward": torch.tensor([0.0], dtype=torch.float32),
                "terminated": torch.tensor([False], dtype=torch.bool),
                "action": torch.zeros((0,0), dtype=torch.int64),
                "state_value": torch.zeros((0,0), dtype=torch.float32),
                "logits": torch.zeros((1, 36), dtype=torch.float32),
                "step_count": torch.zeros((0,0), dtype=torch.int64),
            }, [],
            device="cuda"
        )

        return td

