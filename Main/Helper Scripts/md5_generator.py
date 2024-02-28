import pandas as pd
import hashlib
import sys
import gzip

# Load teams.csv into a DataFrame
teams_file_path = "Challenges\\teams.csv"
df_teams = pd.read_csv(teams_file_path)

# Check if a team number is provided as an argument
if len(sys.argv) != 2:
    print("Usage: python script.py <team_number>")
    sys.exit(1)

# Get the team number from the command line argument
team_number = int(sys.argv[1])

# Check if the team number is within the valid range
if team_number < 0 or team_number >= len(df_teams):
    print("Team number is out of range. Should be between 0 and 49.")
    sys.exit(1)

# Get the CHALLENGE_7_HASH value for the specified team number
challenge_7_hash = str(df_teams.at[team_number, "CHALLENGE_7_HASH"])

# Calculate MD5 hash
md5_hash = hashlib.md5(challenge_7_hash.encode()).hexdigest()

# Write the MD5 hash to a text file
output_file_path = f"Main\\data\\hash.txt"
with open(output_file_path, "w") as output_file:
    output_file.write(md5_hash)

# Compress the text file using gzip at the 9th compression level
compressed_file_path = f"{output_file_path}.gz"
with open(output_file_path, "rb") as file_in:
    with gzip.open(compressed_file_path, "wb", compresslevel=9) as file_out:
        file_out.write(file_in.read())

print(f"MD5 hash for CHALLENGE_7_HASH of Team {team_number} has been calculated and saved to {compressed_file_path}.")
