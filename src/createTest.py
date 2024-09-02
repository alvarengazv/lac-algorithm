import random

def calculate_similarity(line1, line2):
    """Calculate the percentage similarity between two lines."""
    total_digits = len(line1)
    matches = sum(1 for i in range(total_digits) if line1[i] == line2[i])
    return (matches / total_digits) * 100

def find_similar_lines(input_lines, target_similarity, count):
    """Find a specified number of lines with a target similarity."""
    selected_lines = []
    used_indices = set()

    while len(selected_lines) < count:
        random_index = random.randint(0, len(input_lines) - 1)
        if random_index in used_indices:
            continue
        
        reference_line = input_lines[random_index]
        for i, line in enumerate(input_lines):
            if i in used_indices:
                continue
            similarity = calculate_similarity(reference_line, line)
            if similarity >= target_similarity:
                selected_lines.append(line)
                used_indices.add(i)
                if len(selected_lines) >= count:
                    break
    
    return selected_lines

def main(input_filename, output_filename):
    # Read the input file
    with open(input_filename, 'r') as f:
        input_lines = [line.strip().split(',') for line in f.readlines()]

    # Convert lines to list of integers
    input_lines = [[int(num) for num in line] for line in input_lines]

    # Find 25 lines with 80% similarity
    lines_80_percent = find_similar_lines(input_lines, 80, 25)

    # Find 25 lines with 50% similarity
    lines_50_percent = find_similar_lines(input_lines, 50, 25)

    # Select 50 random lines
    random_lines = random.sample(input_lines, 50)

    # Write the results to the output file
    with open(output_filename, 'w') as f:
        for line in lines_80_percent + lines_50_percent + random_lines:
            f.write(','.join(map(str, line)) + '\n')

if __name__ == "__main__":
    input_filename = "../datasets/poker-hand-testing.data"  # Replace with your input file name
    output_filename = "../datasets/poker-hand-testing-with-similarity.data"  # Replace with your output file name
    main(input_filename, output_filename)
