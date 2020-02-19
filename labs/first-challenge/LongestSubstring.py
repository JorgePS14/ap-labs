import sys

def longestSubstringLength(string):
	startIndex = 0
	maxLen = 0
	seenElements = {}

	for i, char in enumerate(string):

		if char in seenElements:
			startIndex = max(startIndex, seenElements[char]+1)

		seenElements[char] = i

		maxLen = max(maxLen, i - startIndex + 1)

	return maxLen

print("Longest substring length in " + sys.argv[1] + " is: " + str(longestSubstringLength(sys.argv[1])))
