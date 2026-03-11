import numpy as np
import gymnasium as gym
from gymnasium import spaces
import jass_engine

class JassEnv(gym.Env):
    metadata = {'render.modes': ['ansi']}

    def __init__(self):
        super().__init__()

        self.game = jass_engine.Game()
        self.action_space = spaces.Discrete(36)
        self.observation_space = spaces.Box(low=0, high=255, shape=(314, ), dtype=np.float32)

    def reset(self, seed=None, options=None):
        super().reset(seed=seed)

        obs = self.game.reset()
        obs = np.array(obs, dtype=np.float32)

        info = {"action_mask": self._get_action_mask()}
        return obs, info

    def step(self, action: int):
        result = self.game.step(action)

        obs = np.array(result.nextObservation, dtype=np.float32)
        reward = float(result.reward)
        terminated = bool(result.done)
        truncated = False

        info = {"action_mask": self._get_action_mask()}
        return obs, reward, terminated, truncated, info

    def _get_action_mask(self) -> np.ndarray:
        mask = np.zeros(36, dtype=bool)

        for card_idx in range(36):
            mask[card_idx] = self.game.checkLegalMove(card_idx)

        return mask

    def render(self):
        print("Test gym render")
