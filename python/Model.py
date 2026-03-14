from sb3_contrib import MaskablePPO
from sb3_contrib.common.wrappers import ActionMasker
from Jass_env import JassEnv

def get_action_mask(env: JassEnv):
    return env._get_action_mask()

def main():
    env = JassEnv()

    env = ActionMasker(env, get_action_mask)

    model = MaskablePPO(
        policy="MlpPolicy",
        env=env,
        verbose=1,
        device='cuda',
        learning_rate=3e-4,
        n_steps=2048,
        batch_size=64,
        n_epochs=10,
        gamma=0.99,
    )

    print("Start training...")
    model.learn(total_timesteps=1000000)

    model.save("jass_agent")
    print("Training finished")

if __name__ == "__main__":
    main()


