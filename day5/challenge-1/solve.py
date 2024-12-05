with open("../puzzle.txt") as f:
    puzzle_input = f.read().split("\n")
page_rules = []
updates = []
for i, line in enumerate(puzzle_input):
    if "|" in line:
        page_rules.append([int(num) for num in line.split("|")])
    elif "," in line:
        updates.append([int(num) for num in line.split(",")])

def isrule_match(page_rules, a, b):
    for i in range(0, len(page_rules)):
        if page_rules[i][0] == a and page_rules[i][1] == b:
            return True
    return False

def iscorrect_update(page_rules, update):
    for i in range(0, len(update)-1):
        if not isrule_match(page_rules, update[i], update[i+1]):
            return False
    return True
def find_correctupdates(page_rules, updates):
    correct_updates = []
    for update in updates:
        if iscorrect_update(page_rules, update):
            correct_updates.append(update)
    return correct_updates
def calculate_middlepage_total(page_rules, updates):
    total_middlepages = 0
    correct_updates = find_correctupdates(page_rules, updates)
    print(correct_updates)
    for update in correct_updates:
        total_middlepages += update[len(update) // 2];
    print("The total middle pages are", total_middlepages)
print("Page ordering rules:")
print(page_rules)
print("Updates:")
print(updates)
calculate_middlepage_total(page_rules, updates)

