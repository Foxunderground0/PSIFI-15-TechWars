import pandas as pd
import sys

def get_team_keys(team_id):
    # Load teams.csv into a DataFrame
    teams_file_path = "Challenges\\teams.csv"
    df_teams = pd.read_csv(teams_file_path)

    # Check if the team_id is valid
    if 0 <= team_id < len(df_teams):
        team_keys = [
            df_teams.at[team_id, "CHALLENGE_3_HASH"],
            df_teams.at[team_id, "CHALLENGE_4_HASH"],
            df_teams.at[team_id, "CHALLENGE_5_HASH"],
            df_teams.at[team_id, "CHALLENGE_2_HASH"],
            df_teams.at[team_id, "CHALLENGE_6_HASH"],
            df_teams.at[team_id, "CHALLENGE_1_HASH"],
            df_teams.at[team_id, "CHALLENGE_7_HASH"]
        ]

        # Save the keys to a text file
        output_file_path = f"Main\\data\\keys.txt"
        with open(output_file_path, "w") as file:
            for key in team_keys:
                file.write(f"{key}\n")

        print(f"Keys for Team {team_id} saved to {output_file_path}")
    else:
        print("Invalid team_id. Please provide a valid team_id in the range 0 to 49.")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <team_id>")
    else:
        team_id = int(sys.argv[1])
        get_team_keys(team_id)
