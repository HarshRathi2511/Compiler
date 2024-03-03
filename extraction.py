def process_file(filename):
    """
    This function processes a file containing grammar rules, separates terminals and non-terminals,
    and prints them in a paragraph with commas, preserving their first occurrence order.

    Args:
      filename: The name of the file to be processed.
    """

    terminals = []
    non_terminals = []

    with open(filename, 'r') as file:
        for line in file:
            for word in line.split():
                if word.startswith("<"):
                    if word not in non_terminals:
                        non_terminals.append(word)
                elif word.startswith("TK_"):
                    if word not in terminals:
                        terminals.append(word)

    # Print terminals and non-terminals in paragraph format with commas
    print("Terminals:", ",".join(terminals))
    print("Non-terminals:", ",".join(non_terminals))

    # Print total counts
    print("\nTotal Terminals:", len(terminals))
    print("Total Non-terminals:", len(non_terminals))


# Example usage
process_file("grammar.txt")  # Replace with your actual filename
