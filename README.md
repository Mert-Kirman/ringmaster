# Description

This C program implements an interpreter in C Language that helps Tolkien while writing
his novel ‘The Lord of the Rings’ by keeping track of characters’ inventories.


The program remains ready to receive inputs until the user enters the "exit" command.
It handles 3 input types:

1. **Sentence:** Providing details about characters and their inventories. When provided
with a valid sentence, the program outputs "OK"; otherwise, it prints "INVALID".
2. **Question:** Prompting the program to find information for J.R.R. Tolkien’s writing.
The program accurately provides answers to questions.
3. **Exit Command:** The program terminates.


## Sentences
### Entities
- **Subject(s):** Describes entities or things of interest. Each subject is a single
word that can contain uppercase and lowercase letters, including underscores. Multiple
subjects can be detailed, separated by ‘and’.
- **Item(s):** Refers to objects associated with a non-negative integer quantity. Each
item is a single word that can contain uppercase and lowercase letters, including
underscores. Multiple items can be specified, each accompanied by a non-negative integer,
and separated by ‘and’. The term does not get a plural suffix ‘s’ when it is specified
as plural.
- **Location:** Represents a place or setting using a single word that can contain
uppercase and lowercase letters, including underscores. Describing multiple locations
is not allowed.

### Actions
- **buy:** Subject(s) acquire item(s) from an infinite source. After this action, the
quantity of purchased item(s) in the inventory is increased. 
  - Subject(s) buy Item(s)
  - Gandalf and Gollum buy 6 bread and 4 water


- **buy ... from:** Subject(s) acquire item(s) from another subject, which is a finite
source. If there are multiple buyers, the seller provides the given amount of items to
each buyer. The buy operation is done only if the seller has enough items for all
buyers. Otherwise, no change takes place. In the buy action, there cannot be more than
one seller subject. The seller and buyer cannot be the same person. After this action,
the quantity of purchased item(s) in the inventory for buyers is increased, while for
the seller, it is decreased.
  - Subject(s) buy Item(s) from Subject
  - Gandalf buy 5 bread from Aragorn


- **sell:** Subject(s) give item(s) to an infinite source. After this action, the
quantity of sold item(s) in the inventory is decreased. If the subject(s) do not
have enough item(s), no change takes place.
  - Subject(s) sell Item(s)
  - Gandalf and Gollum sell 2 bread


- **sell ... to:** Subject(s) which are a finite source, provide item(s) to another
subject. If there are multiple sellers, each provides the given amount of items to
the buyer. The sell operation is done only if all the sellers have enough items for
the buyer. Otherwise, nothing happens. In the sell action, there cannot be more
than one buyer subject. The seller and buyer cannot be the same person. After this
action, the quantity of sold item(s) in the inventory for sellers is decreased,
while for the buyer, it is increased.
  - Subject(s) sell Item(s) to Subject
  - Gandalf and Gollum sell 6 bread and 4 water to Aragorn


- **go to:** Subject(s) move to a location. Subject(s) cannot go to multiple
locations at the same time.
  - Subject(s) go to Location
  - Gandalf go to Shire


- It is possible to create action series called Action(s). It starts with a single
action, followed by an ‘and’ and additional actions, optionally. Each action in an
action series is performed from left to right separately.
  - **Action(s):** Frodo and Sam go to Mordor and Gandalf sell 5 bread and 3 sword
  to Aragorn and Legolas buy 3 hairclip


### Conditions

- **at:** Specifies the location where the subject(s) are present. For this
condition to be true, all subjects mentioned should be at the specified location.
  - Subject(s) at Location
  - Gandalf at Shire


- **has:** Indicates possession, stating that the subject has a certain quantity
of an item. For this condition to be true, each subject mentioned should have the
specified amount of the items exactly.
  - Subject(s) has Item(s)
  - Gandalf and Gollum has 6 bread and 4 water


- **has less than:** Describes a condition where the subject possesses an amount
of an item that is less than a specified value. For this condition to be true,
each subject mentioned should have less than the specified amount of the items.
  - Subject(s) has less than Item(s)


- **has more than:** Describes a condition where the subject possesses an amount
of an item that is greater than a specified value. For this condition to be true,
each subject mentioned should have more than the specified amount of the items.


- These condition types are exclusively used within if statements and cannot
exist independently.
- It is possible to create condition series called Condition(s). It starts
with a single condition, followed by an ‘and’ and additional conditions,
optionally. Each condition in a condition series is assessed from left to right
separately. If all conditions are true, the overall condition is considered
true. If any of the conditions is false, the overall condition is considered
false.
  - **Condition(s):** Frodo at Mordor and Gandalf has lower than 10 ring and
  Aragorn and Legolas has 5 map


### Constructing a Sentence

Sentences can be made in 3 different ways:
- **Basic Sentences:** Composed of a set of actions that the program will perform.
  - Legolas and Gimli go to Rivendell and Legolas and Gimli buy 2 elixir and 1 map from
  Arwen


- **Conditional Sentences:** Include actions that will only be executed if specific
conditions are met. If all given conditions are true, then all actions will occur from left
to right.
  - Action(s) if Condition(s)
  - Frodo and Sam go to Mount_Doom if Gollum has 1 the_One_Ring and Gandalf has less than 3
  staff and 2 bread


- **Sequential Sentences:** Combine actions and conditions in a sequence, allowing for a
series of linked instructions. Each sentence is executed separately, following a
left-to-right order.
  - Sentence-1 and Sentence-2 and Sentence-3
  - Frodo and Sam go to Bree and Frodo buy 3 map from Aragorn and Sam sell 2 dagger to
  Legolas if Frodo has more than 2 ring and Legolas and Gimli at Bree **_and_** Frodo and
  Sam go to Rivendell if Aragorn has 5 map and Frodo has less than 5 potion and Sam has 3
  dagger **_and_** Frodo sell 1 potion to Arwen and Legolas and Gimli go to Rivendell


## Questions

- **Quantity asking (total ... ?):** Inquire about the total count of a specific item for
the mentioned subjects. The question is restricted to a single item and cannot involve
multiple items. The result integer should be returned in a single line as the answer.
  - Subject(s) total Item ?
  - Gandalf and Frodo total ring ?


- **Location asking (where ?):** Inquire about the current location of a specified
subject. The question will involve only one subject.
  - Subject where ?


- **Presence in a location (Who at ... ?):** Seeks information about the subjects
present in a specified location. The response provides a list of all subjects located in
the given place in a single line, separated by ‘and’. If there is no one at the specified
location, the output is "NOBODY".
  - Who at Rivendell ?


- **Inventory inquiry (total ?):** This question aims to retrieve information about the
complete inventory of the specified subject. The question is not limited to a specific
item but encompasses all items present in the subject’s inventory. The result provides
details about each item, including its name and quantity, in a single line as the answer,
separated by ‘and’. There will be only one subject asked at each of this question type.
If a subject doesn’t have any items in their inventory, the output is "NOTHING".
  - Subject total ?
  - Gandalf total ? -> 5 ring and 3 staff and 2 bread


## Exit Command

When the "exit" command is entered, the program terminates.

# Requirements

- **GCC (GNU Compiler Collection):** Ensure that GCC is installed on your system to
compile the C code.

- **Make:** Ensure that make is installed on your system to build the project using the
provided Makefile.

# Usage

To compile and run the program, follow these steps:
1. **Navigate to Project Directory:** Open your terminal and navigate to the project
directory where the Makefile is located.


2. **Compile Using Makefile:** Run the following command to compile the project using the
Makefile.
```console
make
```
This command will execute the following line from the Makefile:
```console
gcc -o ringmaster ./ringmaster.c
```


3. **Run the Program:** After successful compilation, execute the compiled program.
```console
./ringmaster
```

# Notes

- All words are case-sensitive; For example: ‘Shire’ and ‘shire’ are not the same.
- Some example test-cases are provided in the repository