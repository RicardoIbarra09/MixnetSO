import os
import json
import random
import string
from datetime import datetime
import argparse

# Función para generar un ID de votante aleatorio
def generate_voter_id():
    return ''.join(random.choices(string.digits, k=5))

# Función para generar un voto aleatorio
def generate_vote():
    candidates = ['candidate_1', 'candidate_2', 'candidate_3']
    return random.choice(candidates)

# Función para generar una firma digital ficticia
def generate_signature():
    return ''.join(random.choices(string.ascii_letters + string.digits, k=64))

# Función para generar la marca de tiempo actual
def generate_timestamp():
    return datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%SZ')

# Función para generar un voto en formato JSON
def generate_vote_json(voter_id, vote, timestamp, signature):
    vote_data = {
        "voter_id": voter_id,
        "vote": vote,
        "timestamp": timestamp,
        "signature": signature
    }
    return vote_data

# Función principal para generar n votos y guardarlos en archivos JSON
def generate_votes_files(n, output_dir):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    for i in range(n):
        voter_id = generate_voter_id()
        vote = generate_vote()
        timestamp = generate_timestamp()
        signature = generate_signature()
        
        vote_data = generate_vote_json(voter_id, vote, timestamp, signature)
        
        file_name = f"vote_{i+1}.json"
        file_path = os.path.join(output_dir, file_name)
        
        with open(file_path, 'w') as file:
            json.dump(vote_data, file, indent=4)
        
        print(f"Generated {file_name}")

# Función para parsear los argumentos de la línea de comandos
def parse_arguments():
    parser = argparse.ArgumentParser(description='Generate JSON files simulating votes.')
    parser.add_argument('-n', '--number', type=int, required=True, help='Number of votes to generate')
    parser.add_argument('-o', '--output_dir', type=str, required=True, help='Output directory for JSON files')
    return parser.parse_args()

if __name__ == "__main__":
    args = parse_arguments()
    generate_votes_files(args.number, args.output_dir)
