import torch
from tensordict import TensorDict

from torchrl.data import Composite, UnboundedContinuous, Binary, Categorical
from torchrl.envs import EnvBase
from torchrl.envs.utils import check_env_specs

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
                "critic_observation": torch.tensor(result.nextCriticObservation, dtype=torch.float32),
                "action_mask": torch.tensor(result.mask, dtype=torch.bool),
                "reward": torch.tensor(result.reward, dtype=torch.float32),
                "terminated": torch.tensor([result.done], dtype=torch.bool),
            },
        tensordict.shape)

    def _reset(self, tensordict):
        result = self.engine.reset()

        return TensorDict(
            {
                "observation": torch.tensor(result.nextObservation, dtype=torch.float32),
                "critic_observation": torch.tensor(result.nextCriticObservation, dtype=torch.float32),
                "action_mask": torch.tensor(result.mask, dtype=torch.bool),
                "reward": torch.tensor(result.reward, dtype=torch.float32),
                "terminated": torch.tensor([result.done], dtype=torch.bool),
            },
        tensordict.shape)

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
            mask=Binary(
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

        self.done_spec = Composite(
            done=Binary(shape=torch.Size([*self.batch_size, 1]), dtype=torch.bool),
            terminated=Binary(shape=torch.Size([*self.batch_size, 1]), dtype=torch.bool),
            shape=self.batch_size
        )

env = JassEnv()
check_env_specs(env)