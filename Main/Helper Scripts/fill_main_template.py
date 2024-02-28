import pandas as pd
import sys

# Load teams.csv into a DataFrame
teams_file_path = "Challenges\\teams.csv"
df_teams = pd.read_csv(teams_file_path)

# Check if a team id is provided as an argument
if len(sys.argv) != 2:
    print("Usage: python script.py <team_id>")
    sys.exit(1)

# Get the team id from the command line argument
team_id = int(sys.argv[1])

# Check if the team id is within the valid range
if team_id < 0 or team_id >= len(df_teams):
    print("Team id is out of range. Should be between 0 and 49.")
    sys.exit(1)

# Get the values from the CSV for the specified team id
name_hash = str(df_teams.at[team_id, "NAME_HASH"])
challenge_5_hash = str(df_teams.at[team_id, "CHALLENGE_5_HASH"])
challenge_6_hash = str(df_teams.at[team_id, "CHALLENGE_6_HASH"])
team_name = str(df_teams.at[team_id, "TAG"])

# Pad the PWD with 0's until it is 8 characters long at the end
name_hash_padded = name_hash + "0" * (8 - len(name_hash))

# Read the Main_template.ino file
template_file_path = "Main\\Main_template.txt"
with open(template_file_path, "r") as template_file:
    template_content = template_file.read()

# Replace placeholders with values
template_content = template_content.replace("$$PASS$$", name_hash_padded)
template_content = template_content.replace("$$MORSE$$", challenge_5_hash)
template_content = template_content.replace("$$BINARY$$", challenge_6_hash)
template_content = template_content.replace("$$TEAM_NAME$$", team_name)

# Save the modified content to a new file
output_file_path = f"Main\\Main.ino"
with open(output_file_path, "w") as output_file:
    output_file.write(template_content)

print(f"Code for Team {team_id} has been generated and saved to {output_file_path}.")
