import csv
import sys


def main():

    if len(sys.argv) != 3:
        print("Usage: python dna.py [csv file] [txt file]")
        sys.exit(1)

    f = open(sys.argv[1], "r")
    reader = csv.reader(f)
    
    # lines[0] = name and the STRs
    # lines[1] and forward = name and counts
    lines = []
    for row in reader:
        lines.append(row)

    f.close()

    # Read DNA sequence file into a variable
    sequencefile = open(sys.argv[2], "r")
    sequence = sequencefile.readline()
    sequencefile.close()

    # Find longest match of each STR in DNA sequence
    STRS = lines[0]
    longlist = []
    for i in range(1, len(STRS)):
        STR = STRS[i]
        longest = longest_match(sequence, STR)
        longlist.append(longest)

    # Look at values for each name
    for j in range(1, len(lines)):
        name = lines[j]
        namestr = name.pop(0)

        for x in range(len(name)):
            name[x] = int(name[x])

        if longlist == name:
            print(namestr)
            return
    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1
            
            # If there is no match in the substring
            else:
                break
        
        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
