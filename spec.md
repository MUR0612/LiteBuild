|     |     |      |               | The           | University  |            | of Queensland |             |            |       |     |     |
| --- | --- | ---- | ------------- | ------------- | ----------- | ---------- | ------------- | ----------- | ---------- | ----- | --- | --- |
|     |     |      | School        | of Electrical | Engineering |            | and           | Computer    | Science    |       |     |     |
|     |     |      |               | CSSE7231      |             | – Semester |               | 1, 2026     |            |       |     |     |
|     |     |      |               | Assignment    |             | 2          | (Version      | 1.0)        |            |       |     |     |
|     |     |      |               |               |             | Marks:     | 75            |             |            |       |     |     |
|     |     |      |               |               | Weighting:  |            | 15%           |             |            |       |     |     |
|     |     |      |               | Due: 3:00pm   |             | Thursday   |               | 7 May, 2026 |            |       |     |     |
|     |     | This | specification | was           | created     | for the    | use of        | Frank WU    | (s4893827) | only. |     |     |
Do not share this document. Sharing this document may result in a misconduct penalty.
Introduction
1
The goal of this assignment is to demonstrate your skills and ability in fundamental process management
2
and communication concepts (pipes and signals), and to further develop your C programming skills with a
3
| moderately | complex | program. |     |     |     |     |     |     |     |     |     |     |
| ---------- | ------- | -------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
4
You are to write a program called uqmakelite that implements a subset of the features of make(1). Like
5
make(1), your uqmakelite program should be able to build an executable from some source files and only
6
rebuild programs when the executable and/or object files are out of date with respect to the source files.
7
The assignment will also test your ability to code to a particular programming style guide and to use a
8
| revision | control | system | appropriately. |     |     |     |     |     |     |     |     |     |
| -------- | ------- | ------ | -------------- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
9
3219
| Student | Conduct |     |     |     |     |     |     |     |     |     |     |     |
| ------- | ------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
10
Thissectionisunchangedfromassignmentone–butyoushouldremindyourselfofthereferencingrequirements.
Remember that you can’t copy code from websites and if you learn about how to use a library function from a
11
| resource | other | than course-provided |     | material | then | you | must reference |     | it. |     |     |     |
| -------- | ----- | -------------------- | --- | -------- | ---- | --- | -------------- | --- | --- | --- | --- | --- |
assignment. You should feel free to discuss aspects of C programming and
| This is | an individual |     |     |     |     |     |     | general |     |     |     | 12  |
| ------- | ------------- | --- | --- | --- | --- | --- | --- | ------- | --- | --- | --- | --- |
the assignment specification with fellow students, including on the discussion forum. In general, questions like
13
“How should the program behave if hthis happensi?” would be safe, if they are seeking clarification on the
14
specification.
15
Youmustnotactivelyhelp(orseekhelpfrom)otherstudentsorotherpeoplewiththeactualdesign,structure
16
and/or coding of your assignment solution. It is cheating to look at another person’s assignment code
17
| and it is |     |     |     |     |     |     |     |     |     |     | others. |     |
| --------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | ------- | --- |
cheating to allow your code to be seen or shared in printed or electronic form by
18
All submitted code will be subject to automated checks for plagiarism and collusion. If we detect plagiarism
19
or collusion, formal misconduct actions will be initiated against you, and those you cheated with. If you share
20
your code with another student, even inadvertently, then both of you are in trouble. Do not post your
21
code to a public place such as the course discussion forum or a public code repository. (Code in private posts
22
to the discussion forum is permitted.) You must assume that some students in the course may have very long
23
extensions so do not post your code to any public repository until at least three months after the result release
24
date for the course (or check with the course coordinator if you wish to post it sooner). Do not allow others to
25
access your computer – it is your responsibility to keep your code secure. Never leave your work unattended.
26
You must follow the following code usage and referencing rules for all code committed to your SVN
27
| repository | (not | just | the version | that you | submit): |     |     |     |     |     |     |     |
| ---------- | ---- | ---- | ----------- | -------- | -------- | --- | --- | --- | --- | --- | --- | --- |
28
| Code          | Origin |             |       |               |     |     | Usage/Referencing |     |     |     |     |     |
| ------------- | ------ | ----------- | ----- | ------------- | --- | --- | ----------------- | --- | --- | --- | --- | --- |
| Code provided |        | by teaching | staff | this semester |     |     | Permitted         |     |     |     |     |     |
Code provided to you in writing this semester by Maybeusedfreelywithoutreference. (Youmustbeable
CSSE7231teachingstaff(e.g.,codehostedonBlackboard, to point to the source if queried about it – so you may
| found in |     |     |     |     | on  | moss, | find it | easier to | reference the | code.) |     |     |
| -------- | --- | --- | --- | --- | --- | ----- | ------- | --------- | ------------- | ------ | --- | --- |
/local/courses/csse2310/resources
postedonthediscussionforumbyteachingstaff,provided
| in Ed Lessons, |            | or shown | in class).    |     |     |     |     |     |     |     |             |     |
| -------------- | ---------- | -------- | ------------- | --- | --- | --- | --- | --- | --- | --- | ----------- | --- |
| © 2026 The     | University |          | of Queensland |     |     | 1   |     |     |     |     | Version 1.0 |     |
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

| Code | Origin |       |      |          |     |             |     |     | Usage/Referencing |     |     |     |     |
| ---- | ------ | ----- | ---- | -------- | --- | ----------- | --- | --- | ----------------- | --- | --- | --- | --- |
| Code | you    | wrote | this | semester | for | this course |     |     | Permitted         |     |     |     |     |
Code you have personally written this semester for Maybeusedfreelywithoutreference. (Thisassumesthat
CSSE7231(e.g., codewrittenforA1reusedinA2orA3) noreferencewasrequiredfortheoriginaluse. Iftheorig-
– provided you have not shared or published it. inal use required a reference then a reference must be
|     |     |     |     |     |     |     |     |     | added for | this use.) |     |     |     |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --------- | ---------- | --- | --- | --- |
Unpublished code you wrote earlier Conditions apply, references required
| Codeyouhavepersonally |     |     |     | writteninapreviousenrolment |     |     |     |     |             |              |             |          |     |
| --------------------- | --- | --- | --- | --------------------------- | --- | --- | --- | --- | ----------- | ------------ | ----------- | -------- | --- |
|                       |     |     |     |                             |     |     |     |     | May be used | (i.e. copied | or adapted) | provided | you |
inthiscourseorinanotherUQcourseorforotherreasons
|     |     |     |     |     |     |     |     |     | understand | the code AND | the | source of | the code or |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | ---------- | ------------ | --- | --------- | ----------- |
and where that code has not been shared with any other learning is correctly referenced in a comment adjacent to
person or published in any way. that code. For AI use, there must also be an overall
commentineachofyoursourcefilesthatusessuchcode.
| Code | from | man | pages | on moss |     |     |     |     |     |     |     |     |     |
| ---- | ---- | --- | ----- | ------- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
Code examples found in pages on moss. (This does All references must be in the required comment
man
notapplytocodefrommanpagesfoundonothersystems format in the correct place in your files – see the
|     |     |     |     |     |     |     |     |     | style guide | for details. | If  | such code | is used without |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | ----------- | ------------ | --- | --------- | --------------- |
orwebsitesunlessthatcodeisalsointhemossmanpage.)
|      |     |          |      |     |       |     |     |     | correct referencing | then | this | will be considered |     |
| ---- | --- | -------- | ---- | --- | ----- | --- | --- | --- | ------------------- | ---- | ---- | ------------------ | --- |
| Code | and | learning | from | AI  | tools |     |     |     | misconduct.         |      |      |                    |     |
Codewrittenby,modifiedby,debuggedby,explainedby,
| obtained     |            | from,    | or based | on         | the output  | of,      | an        | artificial |     |     |     |     |     |
| ------------ | ---------- | -------- | -------- | ---------- | ----------- | -------- | --------- | ---------- | --- | --- | --- | --- | --- |
| intelligence |            | tool     | or other | code       | generation  |          | tool that | you        |     |     |     |     |     |
| alone        | personally |          | have     | interacted | with,       | without  | the       | assis-     |     |     |     |     |     |
| tance        | of         | another  | person.  | This       | includes    | code     | you       | wrote      |     |     |     |     |     |
| yourself     |            | but then | modified | or         | debugged    | because  |           | of your    |     |     |     |     |     |
| interaction  |            | with     | such     | a tool.    | It also     | includes | code      | you        |     |     |     |     |     |
| wrote        | where      | you      | learned  | about      | the         | concepts | or        | library    |     |     |     |     |     |
| functions    |            | etc.     | because  | of your    | interaction |          | with      | such       | a   |     |     |     |     |
tool. Italsoincludeswherecommentsarewrittenbysuch
| a    | tool – | comments | are     | part of | your      | code. |       |     |            |     |     |     |     |
| ---- | ------ | -------- | ------- | ------- | --------- | ----- | ----- | --- | ---------- | --- | --- | --- | --- |
| Code | copied | from     | sources | not     | mentioned |       | above |     | Prohibited |     |     |     |     |
Code, in any programming language: May not be used. If the source of the code is correctly
• copiedfromanywebsiteorforum(includingStack- referenced adjacent to the code then this will be consid-
Overflow and CSDN); ered code without academic merit (not misconduct) and
• copied from any public or private repositories; will be removed from your assignment prior to marking
• copied from textbooks, publications, videos, apps; (which may cause compilation to fail and zero marks to
• copied from code provided by teaching staff only beawarded). Copiedcodewithoutcorrectreferencingwill
in a previous offering of this course (e.g. previous be considered misconduct and action will be taken.
|     | assignment |            | one             | solution);     |          |            |                |         |                  |              |      |           |                   |
| --- | ---------- | ---------- | --------------- | -------------- | -------- | ---------- | -------------- | ------- | ---------------- | ------------ | ---- | --------- | ----------------- |
|     | • written  |            | by or partially |                | written  | by someone |                | else or |                  |              |      |           |                   |
|     |            |            |                 |                |          |            |                |         | This prohibition | includes     | code | written   | in other program- |
|     | written    |            | with the        | assistance     |          | of someone | else           | (other  |                  |              |      |           |                   |
|     |            |            |                 |                |          |            |                |         | ming languages   | that has     | been | converted | to C.             |
|     | than       | a teaching |                 | staff member); |          |            |                |         |                  |              |      |           |                   |
|     | • written  |            | by an AI        | tool           | that you | did        | not personally |         |                  |              |      |           |                   |
|     |            |            |                 |                |          |            |                |         | While you        | may not copy | such | code, you | may be able to    |
|     | and        | solely     | interact        | with;          |          |            |                |         |                  |              |      |           |                   |
• written by you and available to other students; or learn from it. See the following row in the table.
|     | • from  | any   | other | source | besides | those | mentioned | in  |     |     |     |     |     |
| --- | ------- | ----- | ----- | ------ | ------- | ----- | --------- | --- | --- | --- | --- | --- | --- |
|     | earlier | table | rows  | above. |         |       |           |     |     |     |     |     |     |
Code or resources that you have learned from, other Conditions apply, references required
thanstaffprovidedcode,manpagesandAItooloutput May be used provided you do not directly copy code
Examples, websites, discussions, videos, code (in any ANDyouunderstandthecodeANDthesourceofthein-
programming language), etc. that you have learned from spirationorlearningiscorrectlyreferencedinacomment
orthatyouhavetakeninspirationfromorbasedanypart adjacent to that code (in the required format – see the
of your code on but have not copied or just converted style guide). If such code is used without correct refer-
from another programming language. This includes encing then this will be considered misconduct. Copying
learning about the existence of and behaviour of library the code itself is prohibited – see the row above this one
| functions |     | and | system | calls that | are | not covered | in  | class. | in this table. |     |     |     |     |
| --------- | --- | --- | ------ | ---------- | --- | ----------- | --- | ------ | -------------- | --- | --- | --- | --- |
You must not share this assignment specification or any part of it with any person, organisation,
29
| website, | etc., | with | the | following | exceptions: |     |     |     |     |     |     |     |     |
| -------- | ----- | ---- | --- | --------- | ----------- | --- | --- | --- | --- | --- | --- | --- | --- |
30
|     | • You | are permitted |     | to  | show | this specification |     | to  | course staff. |     |     |     |     |
| --- | ----- | ------------- | --- | --- | ---- | ------------------ | --- | --- | ------------- | --- | --- | --- | --- |
31
• You are permitted to post small extracts of this specification to the course Ed Discussion forum for the
32
purposesofseekingorprovidingclarificationonthisspecification. (Youshouldnotethatyourspecification
33
|     | may | differ | to that | of other | students.) |     |     |     |     |     |     |     |     |
| --- | --- | ------ | ------- | -------- | ---------- | --- | --- | --- | --- | --- | --- | --- | --- |
34
| © 2026 | The | University |     | of Queensland |     |     |     |     | 2   |     |     |     | Version 1.0 |
| ------ | --- | ---------- | --- | ------------- | --- | --- | --- | --- | --- | --- | --- | --- | ----------- |
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

• You are permitted to upload the specification or parts of it to AI and machine translation services but
35
youmustbeawareofandfollowtheAIreferencingrequirements(seethestyleguide),andyoushouldnot
36
trust the output of any of these services to be correct.
37
In short – Don’t risk it! If you’re having trouble, seek help early from a member of the teaching staff.
38
Don’t be tempted to copy another student’s code or to use an online cheating service (many services have
39
themselvesprovidedevidenceofthistotheUniversityinthepast). Don’thelpanotherCSSE2310/7231student
40
with their code no matter how desperate they may be and no matter how close your relationship. You should
41
read and understand the statements on student misconduct in the course profile and on the school website:
42
https://eecs.uq.edu.au/current-students/guidelines-and-policies-students/student-conduct.
43
Specification
44
The uqmakelite program will read a UQmakefile (by default) and then carry out the actions described in that
45
file (details below).
46
In all cases, your program is required to mimic the behaviour of the demo program, which can be run on
47
moss with the command demo-uqmakelite.
48
Command Line Arguments
49
Your uqmakelite program is to accept command line arguments as follows:
50
./uqmakelite [--file filename] [--simulate] [var=value]... [target]...
51
The square brackets ([]) indicate optional arguments. The italics indicate placeholders for user-supplied
52
value arguments. The ellipsis (...) indicates the preceding argument can be repeated any number of times.
53
uqmakelite expects zero or more option arguments beginning with “--” to follow the command name. The
54
--fileoptionargumentmustbefollowedbyavalueargument. Optionargumentscanappearinanyorder. Any
55
remaining arguments (i.e., from the first non-option argument) are interpreted as variable definition arguments
56
iftheycontainanequalssignortargetnamesiftheydon’t. Variabledefinitionargumentsandtargetarguments
57
can appear in any order after the option arguments.
58
Some examples of how the program might be run include the following1:
59
./uqmakelite
60
./uqmakelite --file mymakefile uqsynonym
61
./uqmakelite --simulate COMPILER=clang LINKFLAGS=-lm
62
./uqmakelite "COMPILEFLAGS = -Wall -Wextra" uqsynonym.o thesaurus.o
63
The meaning of the option arguments is as follows. More details on the expected behaviour of the program
64
are provided later in this document.
65
3219
• --file – this option argument specifies that the following value argument (which must be present if this
66
option argument is present) is the name of a file containing variable definitions, dependencies etc. that
67
will describe the expected behaviour of uqmakelite. (This file is called a uqmakefile in the remainder of
68
this specification.) If this option argument is absent then the file UQmakefile (in the current directory)
69
is to be used as the uqmakefile.
70
• --simulate – this option argument specifies that uqmakelite isn’t to build the targets but should just
71
print the commands it would execute without this argument present (i.e., simulate execution)
72
• var=value – arguments of this type specify that the given uqmakelite variable must be assigned the
73
given value. The expected format of such arguments is identical to variable definition lines in uqmakefiles
74
(described on page 5). Variable definitions on the command line are treated as if they come after all
75
variable definitions in the uqmakefile.
76
• target – arguments of this type specify the files or “phony” targets that are to be “built” (or attempted
77
to be built) by uqmakelite – in the order listed.
78
Before doing anything else, your program must check the command line arguments for validity. If the
79
program receives an invalid command line, then it must print the following message:
80
1This is not an exhaustive list and does not show all possible combinations of arguments. The examples assume the existence
ofthefilenames.
© 2026 The University of Queensland 3 Version 1.0
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

Usage: uqmakelite [--file filename] [--simulate] [var=value]... [target]...
81
to standard error (with a following newline), and exit with an exit status of 1.
82
Invalid command lines include (but may not be limited to) any of the following:
83
• The --file option argument is given but is not followed by a value argument (the filename).
84
• Any of the option arguments is listed more than once.
85
• An unexpected option argument is present.
86
3219
• Any argument is an empty string, i.e. has zero length.
87
Option arguments are those prior to the first variable definition or target argument. An argument starting
88
with -- after the first variable definition or target argument is not an option argument – it will be considered
89
an invalid variable definition if an equals sign is present, or a target name (which will also be invalid) if an
90
equals sign is not present.
91
Checkingwhetherauqmakefileexistsorcanbeopenedisnotpartoftheusagechecking(otherthanchecking
92
thatthestringthatfollows--fileisnotanemptystring). Checkingthatvariabledefinitionsand/ortargetsare
93
formatted correctly is not part of the usage checking (other than checking that such arguments are non-empty
94
strings).
95
File Readability Checking
96
If there are no usage errors then your program must check it can open the uqmakefile – either the file specified
97
on the command line, or, if no --file argument is given, then the default uqmakefile (UQmakefile) in the
98
current directory. If the file cannot be opened for reading, then your program must print the message:
99
uqmakelite: unable to open uqmakefile "filename" for reading
100
to standard error (with a following newline) where filename is replaced by the name of the file from the
101
command line or UQmakefile as applicable. The double quotes must be present. Your program must then exit
102
with an exit status of 3.
103
Variable Definitions
104
Iftherearenousageorfileopeningerrorsthenyourprogrammustcheckthevariabledefinitionsonthecommand
105
line (if any) for validity. Any command line argument after the option arguments (if present) that contains
106
an equals sign is considered a potential variable definition and should be checked for validity – based on the
107
requirements described below (from line 154) for variable definition lines in uqmakefiles. (Other arguments are
108
considered potential targets.) If any argument containing an equals sign is not a valid variable definition, then
109
your program must print the message:
110
uqmakelite: invalid variable definition: "variable-definition"
111
to stderr (with a following newline) where variable-definition is replaced by the argument from the
112
command line. The double quotes must be present. This must be printed for all invalid variable definitions on
113
the command line, in the order that they appear on the command line. Your program must then exit with exit
114
status 6. A helper function is provided to help with this task. Note that this check does not involve expanding
115
the variable definitions – that takes place later.
116
File Format Checking
117
If the checks above (as applicable) are successful, then uqmakelite must check the uqmakefile for validity. The
118
required checks are described on page 6 after we first describe the file format itself. Note that helper functions
119
are provided to parse lines into tokens and check that tokens are valid - see the Provided Library section later.
120
UQmakefile File Format
121
Before describing the format of lines in a uqmakefile, we define some terms that are used. Underlined terms
122
mean the previous definition of that term applies.
123
• whitespace character
124
Any character for which the isspace() library function returns true. (In practice, newline characters
125
won’t be found within lines since these separate lines).
126
© 2026 The University of Queensland 4 Version 1.0
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

•
| identifier |     |     |     |     |     |     |     |     |     |     |     | 127 |
| ---------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
A sequence of one or more characters that starts with a letter (upper or lower case) and is followed by
128
| letters | and/or | numbers |     | and/or | underscore |     | characters | (‘_’). |     |     |     |     |
| ------- | ------ | ------- | --- | ------ | ---------- | --- | ---------- | ------ | --- | --- | --- | --- |
129
• filename
130
A sequence of one or more characters other than whitespace characters or any of the following characters
131
| between |     | the double | quotes: | “:=>/#$∼” |     |     |     |     |     |     |     |     |
| ------- | --- | ---------- | ------- | --------- | --- | --- | --- | --- | --- | --- | --- | --- |
132
• pathname
133
A sequence of one or more characters other than whitespace characters or greater than signs (‘>’) where
134
| the | sequence | may | not | begin with | ‘$’. |     |     |     |     |     |     |     |
| --- | -------- | --- | --- | ---------- | ---- | --- | --- | --- | --- | --- | --- | --- |
135
•
argument
136
Either a pathname, or a variable expansion string of the form $variable-name where variable-name is
137
an identifier.
138
•
| command |            |           |     |           |          |     |        |         |            |     |     | 139 |
| ------- | ---------- | --------- | --- | --------- | -------- | --- | ------ | ------- | ---------- | --- | --- | --- |
| A       | whitespace | character |     | separated | sequence |     | of one | or more | arguments. |     |     |     |
140
| • pipe | symbol |     |     |     |     |     |     |     |     |     |     |     |
| ------ | ------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
141
| The | sequence | of  | characters | =>  |     |     |     |     |     |     |     |     |
| --- | -------- | --- | ---------- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
142
| • command |     | pipeline |     |     |     |     |     |     |     |     |     |     |
| --------- | --- | -------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
143
A sequence of one or more commands separated by the pipe symbol and optional whitespace characters
144
| either | side | of each | pipe | symbol. |     |     |     |     |     |     |     |     |
| ------ | ---- | ------- | ---- | ------- | --- | --- | --- | --- | --- | --- | --- | --- |
145
UQmakefiles consist of lines of text where each line is terminated by a newline character (except perhaps
146
the last line in the file). Any newline character is not considered part of the line. Lines can be of any length
147
| and must | have | one of | the forms | below. |     |     |     |     |     |     |     |     |
| -------- | ---- | ------ | --------- | ------ | --- | --- | --- | --- | --- | --- | --- | --- |
148
| 1. Blank | lines |     |     |     |     |     |     |     |     |     |     |     |
| -------- | ----- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
149
Lines that contain no characters or only whitespace characters are to be ignored.
150
| 2.      |     | lines: | # ... |     |     |     |     |     |     |     |     |     |
| ------- | --- | ------ | ----- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Comment |     |        |       |     |     |     |     |     |     |     |     | 151 |
Linesthatbeginwitha#character(possiblywithleadingwhitespace characters)areconsideredcomment
152
| lines | and | are to | be ignored. |     |     |     |     |     |     |     |     |     |
| ----- | --- | ------ | ----------- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
153
| 3. Variable |     | definition |     | lines: | =    |       |     |     |     |     |     |     |
| ----------- | --- | ---------- | --- | ------ | ---- | ----- | --- | --- | --- | --- | --- | --- |
|             |     |            |     |        | name | value |     |     |     |     |     | 154 |
Linesofthisformdefineavariableinternaltouqmakelite. Whitespace charactersmaybepresentaround
155
| the | variable | name | and | the value. | These | lines | are | are valid | provided: |     |     |     |
| --- | -------- | ---- | --- | ---------- | ----- | ----- | --- | --------- | --------- | --- | --- | --- |
156
• the variable name is an identifier. Note that names are case sensitive, e.g. COMPILER is a different
157
|     | variable | to  | compiler. |     |     |     |     |     |     |     |     |     |
| --- | -------- | --- | --------- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
158
• the value part of the line (from the first non-whitespace character after the equals sign to the last
159
non-whitespace character before the end of the line) takes one of the forms:
160
– the empty string (i.e there are no characters after the equals sign, with the possible exception of
161
|     |     | whitespace | characters); |     | or  |     |     |     |     |     |     |     |
| --- | --- | ---------- | ------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- |
162
|            |     | a list of  | one or | more   | whitespace |     | character            | separated | arguments. |     |     |     |
| ---------- | --- | ---------- | ------ | ------ | ---------- | --- | -------------------- | --------- | ---------- | --- | --- | --- |
|            | –   |            |        |        |            |     |                      |           |            |     |     | 163 |
| 4.         |     |            |        | lines: |            |     | :                    |           |            |     |     |     |
| Dependency |     | definition |        |        | filename   |     | list-of-dependencies |           |            |     |     | 164 |
Lines of this form specify the dependencies (if any) of a given file. Whitespace characters may be present
165
around the filename and the dependencies in the list-of-dependencies. The list-of-dependencies may be
166
empty or will be a whitespace character separated sequence of one or more arguments.
167
| 5.    |     |        |            | line: |                   |     |     |     |           |          |     |     |
| ----- | --- | ------ | ---------- | ----- | ----------------- | --- | --- | --- | --------- | -------- | --- | --- |
| Phony |     | target | definition |       | phony-target-name |     |     |     | ∼ command | pipeline |     |     |
168
Lines of this form specify a “phony“ target (i.e. a target that won’t be built as a file) and the com-
169
mand/pipeline to be run when “building” that target. The takes the form of an
|     |     |     |     |     |     |     |     |     | phony-target-name |     |     | 170 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | ----------------- | --- | --- | --- |
identifier.
171
Library functions will be provided to help you parse UQmakefiles – see the Provided Library section later.
172
| Some       | example    | file | listings      | are shown | below: |     |     |     |     |     |             |     |
| ---------- | ---------- | ---- | ------------- | --------- | ------ | --- | --- | --- | --- | --- | ----------- | --- |
| © 2026 The | University |      | of Queensland |           |        |     |     | 5   |     |     | Version 1.0 |     |
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

1: Example uqmakefile. This is the simplest uqmakefile that can be used to build the provided A1
Example
| solution   | code. | See         | Blackboard | for         | your provided | A1 sample | solution. |     |     |     |
| ---------- | ----- | ----------- | ---------- | ----------- | ------------- | --------- | --------- | --- | --- | --- |
| uqsynonym: |       | uqsynonym.o |            | thesaurus.o | wordset.o     |           |           |     |     |     |
1
| COMPILEFLAGS |     | =   | -I/local/courses/csse2310/include |     |     |     |     |     |     |     |
| ------------ | --- | --- | --------------------------------- | --- | --- | --- | --- | --- | --- | --- |
2
| LINKFLAGS=-L/local/courses/csse2310/lib |     |     |     |     |     | -lcsse2310a1 |     |     |     |     |
| --------------------------------------- | --- | --- | --- | --- | --- | ------------ | --- | --- | --- | --- |
3
Example 2: Example uqmakefile. This example will also build the provided A1 solution code, but includes
more complicated features. This example file can be found in /local/courses/csse2310/etc/A1uqmakefile
on moss.
debug=-g
1
| COMPILER |     | = gcc | $debug |     |     |     |     |     |     |     |
| -------- | --- | ----- | ------ | --- | --- | --- | --- | --- | --- | --- |
2
COMPILEFLAGS = -I/local/courses/csse2310/include $COMPILEFLAGS -std=gnu99
3
| LINKFLAGS=-L/local/courses/csse2310/lib |     |     |     |     |     | -lcsse2310a1 |     |     |     |     |
| --------------------------------------- | --- | --- | --- | --- | --- | ------------ | --- | --- | --- | --- |
4
PROG=uqsynonym
5
| header_files |     | =   | thesaurus.h |     | wordset.h |     |     |     |     |     |
| ------------ | --- | --- | ----------- | --- | --------- | --- | --- | --- | --- | --- |
6
# Note that the use of variable can come before its definition (see last line)
7
| uqsynonym: |     | $OBJS |     |     |     |     |     |     |     |     |
| ---------- | --- | ----- | --- | --- | --- | --- | --- | --- | --- | --- |
8
| wordset.o: |     | wordset.c |     | wordset.h |     |     |     |     |     |     |
| ---------- | --- | --------- | --- | --------- | --- | --- | --- | --- | --- | --- |
9
| thesaurus.o |     | :thesaurus.c |     | thesaurus.h |     |     |     |     |     |     |
| ----------- | --- | ------------ | --- | ----------- | --- | --- | --- | --- | --- | --- |
10
| uqsynonym.o:uqsynonym.c |     |     |     | $header_files |     |     |     |     |     |     |
| ----------------------- | --- | --- | --- | ------------- | --- | --- | --- | --- | --- | --- |
11
| clean | ˜ rm | -f  | $OBJS | $PROG |     |     |     |     |     |     |
| ----- | ---- | --- | ----- | ----- | --- | --- | --- | --- | --- | --- |
12
13
sources = wordset.h wordset.c thesaurus.h thesaurus.c uqsynonym.c
14
| LINE_COUNTER |     | =   | wc -l |     |     |     |     |     |     |     |
| ------------ | --- | --- | ----- | --- | --- | --- | --- | --- | --- | --- |
15
numstyleerrors ˜ 2310stylecheck.sh $sources => grep error: => $LINE_COUNTER
16
| style | ˜ 2310stylehceck.sh |     |     | $sources |     |     |     |     |     |     |
| ----- | ------------------- | --- | --- | -------- | --- | --- | --- | --- | --- | --- |
17
| OBJS | = uqsynonym.o |     | thesaurus.o |     | wordset.o |     |     |     |     |     |
| ---- | ------------- | --- | ----------- | --- | --------- | --- | --- | --- | --- | --- |
18
| Required |     | File Checks |     |     |     |     |     |     |     | 173 |
| -------- | --- | ----------- | --- | --- | --- | --- | --- | --- | --- | --- |
Errors are reported for all invalid lines, not just the first. Lines are checked in order; blank and comment lines
174
are ignored (other than causing the line number counter to be incremented). Checks are carried out in the
175
following order. Helper functions are provided – see the Provided Library section later.
176
• A line that has the equals sign character as its second token should be considered a potential variable
177
definition line and be checked for validity against the rules above. If it is invalid, then uqmakelite is to
178
|     | print | the following |     | message | to stderr | (followed by | a newline): |     |     |     |
| --- | ----- | ------------- | --- | ------- | --------- | ------------ | ----------- | --- | --- | --- |
179
|     | filename: |     | line | linenum: | variable | definition | not | valid |     |     |
| --- | --------- | --- | ---- | -------- | -------- | ---------- | --- | ----- | --- | --- |
180
where filename is replaced by the name of the uqmakefile from the command line or UQmakefile as
181
applicable, and is replaced by the line number on which the problem was found. (The first line
linenum
182
|     | in the | file is | line 1.) |     |     |     |     |     |     |     |
| --- | ------ | ------- | -------- | --- | --- | --- | --- | --- | --- | --- |
183
• A line that has the colon character as its second token should be considered a potential dependency
184
definition line and be checked for validity against the rules above. If it is invalid, then uqmakelite is to
185
|     | print | the following |     | message | to stderr | (followed by | a newline): |     |     |     |
| --- | ----- | ------------- | --- | ------- | --------- | ------------ | ----------- | --- | --- | --- |
186
|     | filename: |     | line | linenum: | invalid | dependency | definition |     |     |     |
| --- | --------- | --- | ---- | -------- | ------- | ---------- | ---------- | --- | --- | --- |
187
|     | where |          |     | and     | are replaced | as above. |     |     |     |     |
| --- | ----- | -------- | --- | ------- | ------------ | --------- | --- | --- | --- | --- |
|     |       | filename |     | linenum |              |           |     |     |     | 188 |
• A line that has the tilde character (‘∼’) as its second token should be considered a potential phony target
189
definition line and be checked for validity against the rules above. If it is invalid, then uqmakelite is to
190
|     | print | the following |     | message | to stderr | (followed by | a newline): |     |     |     |
| --- | ----- | ------------- | --- | ------- | --------- | ------------ | ----------- | --- | --- | --- |
191
|     | filename: |     | line | linenum: | phony | target definition |     | not valid |     |     |
| --- | --------- | --- | ---- | -------- | ----- | ----------------- | --- | --------- | --- | --- |
192
|     | where |          |     | and     | are replaced | as above. |     |     |     |     |
| --- | ----- | -------- | --- | ------- | ------------ | --------- | --- | --- | --- | --- |
|     |       | filename |     | linenum |              |           |     |     |     |     |
193
| © 2026 | The | University |     | of Queensland |     | 6   |     |     | Version 1.0 |     |
| ------ | --- | ---------- | --- | ------------- | --- | --- | --- | --- | ----------- | --- |
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

• Ifalineisavaliddependencydefinitionorphonytargetdefinitionandthenameofthetargetisthesame
194
as that in a previous dependency definition or phony target definition then uqmakelite is to print the
195
|     | following | message |     | to stderr |     | (followed | by  | a newline): |     |     |     |
| --- | --------- | ------- | --- | --------- | --- | --------- | --- | ----------- | --- | --- | --- |
196
|     | filename: |     | line | linenum: |     | target |     | name is | repeated |     |     |
| --- | --------- | --- | ---- | -------- | --- | ------ | --- | ------- | -------- | --- | --- |
197
|     | where |          |     | and |         | are | replaced | as above. |     |     |     |
| --- | ----- | -------- | --- | --- | ------- | --- | -------- | --------- | --- | --- | --- |
|     |       | filename |     |     | linenum |     |          |           |     |     |     |
198
• Any other line determined to be invalid (i.e., it contains fewer than two tokens or the second token is
199
not one of the characters mentioned above) must result in the following message being printed to stderr
200
|     | (followed | by  | a newline): |     |     |     |     |     |     |     |     |
| --- | --------- | --- | ----------- | --- | --- | --- | --- | --- | --- | --- | --- |
201
|     | filename: |     | line | linenum: |     | line | not | valid |     |     |     |
| --- | --------- | --- | ---- | -------- | --- | ---- | --- | ----- | --- | --- | --- |
202
|     | where | filename |     | and | linenum | are | replaced | as above. |     |     |     |
| --- | ----- | -------- | --- | --- | ------- | --- | -------- | --------- | --- | --- | --- |
203
|     | If any | errors | are found, | then | uqmakelite |     | is  | to then | exit with exit status | 4.  |     |
| --- | ------ | ------ | ---------- | ---- | ---------- | --- | --- | ------- | --------------------- | --- | --- |
204
If no dependency or phony target definition lines are found in the file then uqmakelite is to print the
205
| following |     | message | to stderr |     | (followed | by  | a newline): |     |     |     |     |
| --------- | --- | ------- | --------- | --- | --------- | --- | ----------- | --- | --- | --- | --- |
206
|     | filename: |     | no  | targets | were | found | in  | file |     |     |     |
| --- | --------- | --- | --- | ------- | ---- | ----- | --- | ---- | --- | --- | --- |
207
where is replaced as above. uqmakelite is to then exit with exit status 5.
filename 208
| Target |     | Existence |     | Checking |     |     |     |     |     |     |     |
| ------ | --- | --------- | --- | -------- | --- | --- | --- | --- | --- | --- | --- |
209
If all of the checks above succeed and if any targets are listed on the command line then uqmakelite must
210
checkthatthetargetsexistintheuqmakefile–aseitherafilenametargetinadependencydefinitionoraphony
211
target in a phony target definition. For all listed targets that are not found in the file, uqmakelite must print
212
| the | following | message |     | to stderr | (with | a   | following | newline); |     |     |     |
| --- | --------- | ------- | --- | --------- | ----- | --- | --------- | --------- | --- | --- | --- |
213
|     | uqmakelite: |     | unable |     | to find | target |     | "target" | in file "filename" |     |     |
| --- | ----------- | --- | ------ | --- | ------- | ------ | --- | -------- | ------------------ | --- | --- |
214
where is replaced by the name of the target given on the command line and is replaced as
target filename
215
above. The double quotes must be present. Targets are checked in the order they appear on the command line.
216
| If  | one or | more are | not | found | then | uqmakelite |     | is to exit | with status 19. |     |     |
| --- | ------ | -------- | --- | ----- | ---- | ---------- | --- | ---------- | --------------- | --- | --- |
217
| uqmakelite |     | Behaviour |     |     |     |     |     |     |     |     |     |
| ---------- | --- | --------- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
218
If all of the checks above succeed then uqmakelite is to behave as described below.
219
| Variable |     | Value | Determination |     |     |     |     |     |     |     |     |
| -------- | --- | ----- | ------------- | --- | --- | --- | --- | --- | --- | --- | --- |
220
First,variablevaluesaredeterminedasfollows. Variablevaluesaresequences(possiblyempty)ofstrings,where
221
| the | strings | might | be identifiers, |     | pathnames, |     | etc. |     |     |     |     |
| --- | ------- | ----- | --------------- | --- | ---------- | --- | ---- | --- | --- | --- | --- |
222
|     | 1. The | following | variables |     | are given | the | following | initial | values: |     |     |
| --- | ------ | --------- | --------- | --- | --------- | --- | --------- | ------- | ------- | --- | --- |
223
|     | •   | COMPILER |     | = gcc |     |     |     |     |     |     |     |
| --- | --- | -------- | --- | ----- | --- | --- | --- | --- | --- | --- | --- |
224
|     | •   | LINKER | =   | gcc |     |     |     |     |     |     |     |
| --- | --- | ------ | --- | --- | --- | --- | --- | --- | --- | --- | --- |
225
|     | •   | COMPILEFLAGS |     | =   | -Wall | -Wextra | -pedantic |     |     |     |     |
| --- | --- | ------------ | --- | --- | ----- | ------- | --------- | --- | --- | --- | --- |
226
|     | •   | LINKFLAGS |     | =   |     |     |     |     |     |     |     |
| --- | --- | --------- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
227
|     |     | (This | means | there | are no | strings | in  | this variable | value.) |     |     |
| --- | --- | ----- | ----- | ----- | ------ | ------- | --- | ------------- | ------- | --- | --- |
228
2. Any variable definitions in the uqmakefile are applied in the order they appear within the file. (It is
229
possible one or more variable definitions may override the default values above.)
230
3. Any variable definitions on the command line are then applied in the order they appear on the command
231
|     | line. | (Again, | it  | is possible | that | previous |     | definitions | are overridden.) |     |     |
| --- | ----- | ------- | --- | ----------- | ---- | -------- | --- | ----------- | ---------------- | --- | --- |
232
Note that determining the new value of a variable may involve expansion of an existing variable. For
233
example:
234
|     | COMPILEFLAGS |     |     | = $COMPILEFLAGS |     |     | -Werror | $DEBUG |     |     |     |
| --- | ------------ | --- | --- | --------------- | --- | --- | ------- | ------ | --- | --- | --- |
235
would result in two variables being expanded and the resulting sequence of strings becoming the new value for
236
| the | COMPILEFLAGS |     | variable. |     |     |     |     |     |     |     |     |
| --- | ------------ | --- | --------- | --- | --- | --- | --- | --- | --- | --- | --- |
237
| ©   | 2026 The | University |     | of Queensland |     |     |     | 7   |     |     | Version 1.0 |
| --- | -------- | ---------- | --- | ------------- | --- | --- | --- | --- | --- | --- | ----------- |
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

When determining the value of variables, variable expansions are performed at the time their names are
238
encountered. (For other lines in the uqmakefile (dependency and phony target lines), variable expansions are
239
not performed until after all variable definitions are processed and variable values are determined – and then
240
only when necessary, e.g., if that target is being built.) If a variable is defined but its value is empty then
241
nothing is added to the new sequence of strings. If a variable to be expanded is undefined (at the time of its
242
use) then uqmakelite must print a warning message to stderr:
243
Unknown variable: "varname"
244
where varname is replaced by the name of the variable. The double quotes must be present. The program
245
continues as if the variable value was empty, so nothing is added to the new sequence of strings.
246
Target Building
247
Once variable values have all been determined, uqmakelite is to “build” each target specified on the command
248
line. If no targets are specified on the command line, then uqmakelite is to build the first target in the
249
uqmakefile, i.e., the first filename in a dependency line or the first phony target name in a phony target line –
250
whichever occurs first in the file.
251
Targets specified on the command line or the default target are considered “top level” targets. Building
252
thesetargetsmayrequirebuildingtheirdependencies(iftheydon’texistorareoutofdatewithrespecttotheir
253
dependencies)andbuildingthetopleveltargetitselfifanyofitsdependencieswerebuiltorthetopleveltarget
254
is out of date with respect to its dependencies.
255
Note that determining dependencies and determining commands and arguments to run may first involve
256
variable expansion. Variable expansion is only performed if required, i.e., if it is needed for a given target (or
257
a dependency) to be built. Variable expansion in dependency lines and phony target lines occurs as described
258
above for variable definitions (see lines 238 to 246) – including printing a warning message if required. Variable
259
expansion must only ever occur once for a given line, even if a target is “built” more than once (e.g., a phony
260
target listed as a dependency for more than one file).
261
If the --simulate argument is present on the command line then uqmakelite is not to execute the compi-
262
lationorlinkingcommandsorcommandpipelinesassociatedwithphonytargetsbeing“built”–itwillsimulate
263
execution by printing the command that would be executed in the normal way and assume that the command
264
is successful, e.g., the target file is created successfully.
265
For each target to be built, uqmakelite is to behave as follows:
266
1. If the target is the name of a phony target then the command pipeline specified for that phony target is
267
to be executed (see Command Pipelines – Advanced Functionality 1 later).
268
2. If the target is not listed as the name of a filename target on a dependency definition line2, or it is listed
269
as the filename target on a dependency definition line but there are no listed dependencies then:
270
(a) if the target name ends in .o then uqmakelite will act as though the target was listed as a filename
271
target in a dependency line with the corresponding .c file listed as the only dependency – meaning
272
the procedure below (point 3) will be followed.
273
(b) otherwise, itmustbeassumedthatthetargetshouldbeanexistingfile. Ifafilewiththatpathname
274
does not exist then uqmakelite must print the following to stderr (with a following newline):
275
uqmakelite: cannot find file "target" - aborting
276
where target is replaced by the name of the file that could not be found. The double quotes must
277
be present. uqmakelite is then to exit with status 7.
278
If the file exists, no action needs to be taken.
279
3. Ifthetargetisthenameofafilenametargetfromadependencydefinitionlinethathasdependencieslisted
280
then, each dependency listed is to be treated as a target (in the order given) and this whole procedure
281
(fromstep1)istobeappliedtothattarget. Afterthat,ifthetargetfiledoesnotexistoranydependencies
282
are built, or any dependencies are files that do not need building but are newer than this target, or any
283
dependencies are phony targets then this target file will be built as described in the following points,
284
otherwise, nothing is executed by uqmakelite for this target.
285
2Notethatthiswon’thappenfortopleveltargetssinceitischeckedwhethertheyexistornotbeforegettingtothispoint. This
willonlyhappenfordependenciesthatarebeingtreatedastargetsaspartofthisbuildprocess.
© 2026 The University of Queensland 8 Version 1.0
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

(a) Ifthistargetfilenameendsin.othenuqmakeliteistoexecutethecommandthatwouldresultfrom
286
expanding variables in the following:
287
$COMPILER -c $COMPILEFLAGS modified-filename
288
where modified-filename is the target filename with .o replaced by .c.
289
(b) If this target filename does not end in .o then it is assumed to be an executable, and uqmakelite is
290
to execute the command that would result from expanding variables in the following:
291
$LINKER $COMPILEFLAGS -o target-filename dependencies $LINKFLAGS
292
where target-filename is replaced by the target filename, and dependencies is replaced by the
293
names of all of the dependencies on the dependency line (excluding those that are phony targets).
294
(c) uqmakeliteistoprint(tostdout)eachcompilationandlinkingcommandpriortoexecution–with
295
single spaces separating each argument. Each printed line must be terminated by a newline. The
296
stdout and stderr of all compiler and linker commands executed should be sent to uqmakelite’s
297
stdout and stderr respectively.
298
(d) uqmakeliteisalwaystowaitforeachcommandexecutiontofinishbeforeproceeding. Iftheexecution
299
ofanycompilationorlinkingcommandfails(returnsanon-zeroexitstatusordoesnotexitnormally)
300
then uqmakelite is to print the following to stderr (with a following newline):
301
uqmakelite: cannot build target "target" - aborting
302
where target is replaced by the name of the target that couldn’t be built. The double quotes must
303
bepresent. uqmakeliteisthentoexitwiththeexitstatusofthefailedprogram,or99iftheprogram
304
exited due to being signalled, or 98 if the program couldn’t be executed in the first place (e.g., the
305
command itself could not be executed, perhaps because it could not be found in the user’s PATH).
306
A file is “newer” than another if its modification time is later (using nanosecond precision). See Hints later
307
for a function you can use to determine file existence and modification times.
308
If no commands needed to be run to bring a top level filename target up to date (or just printed if the
309
--simulate argument was specified on the command line), then uqmakelite should print
310
Target "target" - no action required
311
to stdout (followed by a newline) where target is replaced by the name of the top-level target. The double
312
quotes must be present.
313
Command Pipelines – Advanced Functionality 1
314
Command pipelines are used in the definition of phony targets.
315
Wheneveracommandpipelineistobeexecuted(i.e.,whenaphonytargetisbeing“built”),onceuqmakelite
316
performs variable expansion, it must first check the validity of the command pipeline before attempting execu-
317
tion3. A command pipeline is valid if there is at least one command to be executed and there are no empty
318
commands. (It is not necessary to check if the given string is indeed an executable program on the user’s
319
PATH – it is only necessary to check that there is indeed a program name that can be attempted to be exe-
320
cuted.) If uqmakelite determines that a command pipeline is invalid, then it must not attempt execution of
321
any commands in the pipeline and must print the following message to stderr (with a following newline):
322
uqmakelite: invalid command pipeline
323
uqmakelite must then exit with exit status 12.
324
Beforeattemptingtheexecutionofanycommandpipeline,thewholecommandpipelinetobeexecutedmust
325
be printed to uqmakelite’s stdout – using single spaces between commands and arguments and around pipe
326
symbols (=>).
327
Once the pipeline is executed, if any command in a command pipeline fails because it was signalled or
328
because it didn’t exist (wasn’t executable) or if the last command fails because it did not exit normally, then
329
uqmakelite is to print the following to stderr (with a following newline):
330
uqmakelite: command pipeline failed
331
uqmakelite must then exit with exit status 13. Note that all commands in the pipeline should be run before
332
any child processes are reaped. Child processes must be reaped as they exit (which may not be the order they
333
were executed in). Note also that a failing pipeline may still produce output, e.g., if the last command in a
334
pipeline produces output even if an earlier one failed.
335
3Note that a command pipeline may be considered valid when a uqmakefile is initially checked but may not be valid when
checkedaftervariableexpansion,e.g. becauseavariableexpandstonostrings.
© 2026 The University of Queensland 9 Version 1.0
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

When a command pipeline is run, the stderr for all commands must be sent to uqmakelite’s stderr. The
336
stdout for the last command must be sent to uqmakelite’s stdout. The stdin for the first command in the
337
pipeline must come from /dev/null.
338
If the --simulate argument is given on the command line, then command pipelines are only printed, not
339
printed and executed. In this case, it must be assumed that all commands executed exit normally with exit
340
status 0.
341
Interrupting uqmakelite – Advanced Functionality 2
342
If uqmakelite receives a SIGINT signal (as usually sent by pressing Ctrl-C4) then it (a) must not run any
343
further commands, (b) if any commands are still running then it must immediately send a SIGINT signal to
344
those commands (and only those commands), or if no commands are running then it must exit, and (c) if
345
any SIGINT signals were sent, then it must wait for those commands to terminate and exit as soon as all
346
have terminated, or, if any tasks are still running one second after sending the SIGINT signal(s) or after the
347
last command termination (whichever is later), then it must send a SIGKILL signal to only those remaining
348
commands and exit.
349
If uqmakelite is exiting and running tasks had to be terminated (i.e., at least one SIGINT was sent), then
350
uqmakelite must print the following message to stderr (terminated with a newline):
351
uqmakelite: execution interrupted by signal
352
and then exit with status 11.
353
Other Requirements
354
Your program must also meet all of the following requirements:
355
• If no errors are encountered, uqmakelite is to exit with exit status 0.
356
• Commands to be run that do not include the ‘/’ character should be searched for on the user’s PATH (by
357
using a ‘p’ variant of an exec...() function.
358
• Ifyourprogramisexitingnormallythenitmustfreeallallocatedmemorybeforeexiting. Thisrequirement
359
to free memory does not apply to child processes of uqmakelite, only to the original process.
360
• Programs exec’d in child processes of uqmakelite must not inherit any open file descriptors opened by
361
uqmakelite. (Open file descriptors that uqmakelite inherits from its parent and that are passed to a
362
child must remain open in the child.)
363
• uqmakeliteisnottoleavebehindanyorphanprocesses(i.e., when uqmakeliteexitsnormallythennone
364
ofitschildrenmuststillberunning). uqmakeliteisalsonottoleavebehindanyzombieprocesses–dead
365
children should be reaped quickly.
366
• uqmakelite must not busy wait, i.e., it should not repeatedly check for something in a loop using a
367
non-blocking call or a short sleep. (Reaping dead children in a loop is fine provided your program doesn’t
368
keep trying to reap dead children once it knows there are currently no more dead children.)
369
• All commands run by uqmakelite must be direct children of uqmakelite, i.e., the use of grandchild
370
processes is not permitted.
371
• uqmakelite must never create any temporary files.
372
For any aspects of behaviour not described in this specification, your program must behave in the same
373
mannerasdemo-uqmakeliteonmoss. Ifyou’reunsureaboutsomeaspect,thenyoucanrunthedemoprogram
374
to check the expected behaviour.
375
Wewillnottestforunexpectedsystemcallorlibraryfunctionfailuresinanotherwisecorrectlyimplemented
376
program (e.g. if malloc(), fork() or pipe() fails due to insufficient available resources).
377
4RememberfromclassthatCtrl-CwillsendSIGINTtothewholeforegroundprocessgroup. Youshouldtestthisfunctionality
byusingkilltosendaSIGINTjusttouqmakelite.
© 2026 The University of Queensland 10 Version 1.0
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

Provided Library
378
A library has been provided to you with the following functions which your program may use:
379
3219
lexer_tokenize_line();
380
free_token_list();
381
is_valid_identifier_token();
382
is_valid_filename_token();
383
is_valid_pathname_token();
384
is_valid_argument_token();
385
is_valid_variable_expansion_token();
386
is_valid_variable_definition();
387
is_valid_dependency_definition();
388
is_valid_phony_target_definition();
389
Refer to the man pages of these functions on moss for details.
390
To use the library, you will need to add #include <csse2310a2.h> to your code and use the compiler flag
391
-I/local/courses/csse2310/includewhencompilingyourcodesothatthecompilercanfindtheincludefile.
392
You will also need to link with the library containing these functions. To do this, use the compiler arguments
393
-L/local/courses/csse2310/lib -lcsse2310a2.
394
3219
Style
395
Your program must follow version 4 of the CSSE2310/CSSE7231 C programming style guide available on
396
the course Blackboard site. There are strict referencing and AI documentation requirements described there.
397
Note that from this assignment, your functionality mark will be capped at 30 out of 60 if you fail to include at
least file level comments in every source file that describe your AI use (or not) in the required Doxygen format.
398
3219 Missing comments will be reported by 2310stylecheck.sh.
Note that a single global variable of type bool may be used in your assignment – for the implementation of
399
signal handling. Any other use of global variables will be heavily penalised – see style marking details on page
400
15.
401
Hints
402
1. Run the demo program (demo-uqmakelite) to see how your program should behave. If your program
403
exactly mimics the demo program, then you should receive full marks for functionality. You can create
404
your own uqmakefiles for testing purposes, and you may wish to use the provided A1 solution code as a
405
test case also.
406
2. Code from week 6 and 7 lectures and Ed Lessons exercises may be useful to you.
407
3. If you haven’t yet reviewed the provided A1 solution, you should do so – the code itself from that may
408
be useful to you, e.g., for parsing command line arguments. The WordSet and Thesaurus types and
409
associated functions may also be useful.
410
4. It is strongly recommended that you use the provided library functions – see above.
411
5. Thesystemcall stat(2)canbeusedtochecktheexistenceofafileandfindoutinformationaboutafile,
412
e.g., its modification time can be found in the st_mtim member of the stat structure – see the man page
413
fordetails. Thisst_mtimmemberisoftypestruct timespecwhichisdefinedinthenanosleep(2)man
414
page.
415
6. Consider a recursive implementation of target building.
416
7. Consider the use of sigtimedwait(2) on SIGCHLD when waiting for a child to exit within one second.
417
8. ReadtheSVNCommitHistoryMarkingsectionbeforemakinganycommitstoyourrepository. Commits
418
or commit messages with errors may cost you marks and cannot be undone.
419
9. For a given process, you can examine the file descriptors that it has open by running
420
ls -l /proc/PID/fd
421
where PID is replaced by the process ID.
422
10. Youcanusethe--trace-children=noand--child-silent-after-fork=yesoptionstovalgrindwhen
423
checking for memory leaks. This will look only at uqmakelite and ignore child processes.
424
3219
© 2026 The University of Queensland 11 Version 1.0
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

11. In gdb, you can use set follow-fork-mode child to follow a child process after fork(). By default,
425
gdb will follow the parent process.
426
Possible Approach
427
It is suggested that you write your program by addressing functionality in the order given for functionality
428
marks (page 14). Test your program at each stage and commit to your SVN repository frequently. Note
429
that the specification text above and the behaviour of the demo program are the definitive descriptions of the
430
expected program behaviour.
431
Forbidden Functions
432
You must not use any of the following C statements/directives/etc. If you do so, you will get zero (0) marks
433
for the assignment.
434
• goto
435
• #pragma
436
3219
• gcc attributes (other than the possible use of __attribute__((unused)) as described in the style guide)
437
You must not use any of the following C functions. If you do so, you will get zero (0) marks for any test case
438
where your code calls the function.
439
• longjmp(3) and equivalent functions
440
• system(3)
441
• popen(3)
442
3219
• mkfifo(3) or mkfifoat(3)
443
• signal(2),bsd_signal(3),sigpending(2),sigqueue(3),sigwaitinfo(2),sigsuspend(2),signalfd(2),
444
sigvec(3), sigmask(3), sigblock(3), sigsetmask(3), siggetmask(3)5
445
• sleep(3), usleep(3), nanosleep(2)
446
• select(2), pselect(2), poll(2), ppoll(2), alarm(2) or any other function that allows a timeout
447
EXCEPT for sigtimedwait(2)
448
• Any pthread* functions
449
• Any sem* semaphore functions
450
• Functionsdescribedinthemanpageasnon-standard,e.g. strcasestr(). Standardfunctionswillconform
451
to a POSIX standard – often listed in the “CONFORMING TO” section of a man page.
452
The use of comments to control the behaviour of clang-format and/or clang-tidy (e.g., to disable style
453
checking) will result in zero marks for automatically marked style.
454
Testing
455
Youareresponsibleforensuringthatyourprogramoperatesaccordingtothespecification. Youareencouraged
456
to test your program on a variety of scenarios. The sample uqmakefile from Example 2 is available on moss at
457
/local/courses/csse2310/etc/A1uqmakefile. You can create your own files for testing as required. (Other
458
files will certainly be used in marking.)
459
A variety of programs will be provided to help you in testing:
460
• A demonstration program (called demo-uqmakelite) that implements the correct behaviour is available
461
on moss. You can test your program with a set of command line arguments and a particular uqmakefile,
462
and also run the same test with demo-uqmakelite to check that you get the same output and exit status.
463
You can also use demo-uqmakelite to check the expected behaviour of the program if some part of this
464
specification is unclear.
465
5Youmayusesigaction(2),sigprocmask(2),sigemptyset(3),sigaddset(3)etc.
© 2026 The University of Queensland 12 Version 1.0
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

• A test script will be provided on moss that will test your program against a subset of the functionality
466
requirements – approximately 50% of the available functionality marks. The script will be made available
467
about7to10daysbeforetheassignmentdeadlineandcanbeusedtogiveyousomeconfidencethatyou’re
468
on the right track. The “public tests” in this test script will not test all functionality, and you should be
469
sure to conduct your own tests based on this specification and the demo program. The “public tests” will
470
be used in marking, along with a set of “private tests” that you will not see until results are released.
471
• The Gradescope submission site will also be made available about 7 to 10 days before the assignment
472
deadline. Gradescope will run the test suite immediately after you submit. When this is complete6 you
473
will be able to see the results of the “public tests”. You should check these test results to make sure your
474
program is working as expected. Behaviour differences of the same code between moss and Gradescope
475
may be due to memory initialisation assumptions in your code, so you should allow enough time to check
476
(and possibly fix) any issues after submission.
477
Submission
478
Yoursubmissionmustincludeallsourceandanyotherrequiredfiles(inparticular,youmustsubmitaMakefile).
479
Do not submit compiled files (e.g., .o files, compiled programs) or test input files.
480
3219
Your program (named uqmakelite) must build on moss.labs.eait.uq.edu.au and in the Gradescope
481
environment with:
482
make
483
Your program must be compiled with gcc with at least the following options:
484
-Wall -Wextra -pedantic -std=gnu99
485
You are not permitted to disable warnings or use pragmas to hide them. You may not use source files other
486
than .c and .h files as part of the build process – such files will be removed before building your program.
487
3219
If any errors result from the make command (i.e., the uqmakelite executable can not be created), then you
488
will receive 0 marks for functionality (see below). Any code without academic merit will be removed from your
489
program before compilation is attempted (and if compilation fails, you will receive 0 marks for functionality).
490
3219
Your program must not invoke other programs or use non-standard headers/libraries.
491
Your assignment submission must be committed to your Subversion repository under
492
svn+ssh://source.eait.uq.edu.au/csse2310-2026-sem1/csse2310-s4893827/trunk/a2
493
Onlyfilesatthistoplevelwillbemarked, sodo not put source files in subdirectories. Youmaycreate
494
subdirectories for other purposes (e.g. your own test files), but these will not be considered in marking – they
495
will not be checked out of your repository.
496
Youmustensurethatallfilesneededtocompileanduseyourassignment(includingaMakefile)arecommit-
497
ted and within the trunk/a2 directory in your repository (and not within a subdirectory), and not just sitting
498
in your working directory. Do not commit compiled files or binaries. You are strongly encouraged to check out
499
a clean copy for testing purposes.
500
To submit your assignment, you must run the command
501
3219
2310createzip a2
502
on moss and then submit the resulting zip file on Blackboard (a GradeScope submission link will be made
503
available in the Assessment area on the CSSE2310/7231 Blackboard site)7. The zip file will be named
504
s4893827_csse2310_a2_timestamp.zip
505
where timestamp is replaced by a timestamp indicating the time that the zip file was created.
506
The2310createziptoolwillcheckoutthelatestversionofyourassignmentfromtheSubversionrepository,
507
ensure it builds with the command ‘make’, and if so, will create a zip file that contains those files and your
508
Subversion commit history and a checksum of the zip file contents. You may be asked for your password as
509
partofthisprocesstocheckoutyoursubmissionfromyourrepository. Youwillbeaskedotherquestionsabout
510
your use of AI and code from elsewhere. You must answer these truthfully.
511
You must not create the zip file using some other mechanism, and you3219must not modify the zip file before
512
submission. If you do so, you will receive zero marks. Your submission time will be the time that the file is
513
6Gradescope marking may take only a few minutes or more than 30 minutes, depending on the functionality and efficiency of
yourcode.
7You may need to use scp or a graphical equivalent such as WinSCP, Filezilla or Cyberduck to download the zip file to your
localcomputerandthenuploadittothesubmissionsite.
© 2026 The University of Queensland 13 Version 1.0
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

submitted via GradeScope on Blackboard, and not the time of your last repository commit nor the time of the
514
creation of your submission zip file.
515
MultiplesubmissionstoGradescopearepermitted. Wewillmarkwhicheversubmissionyouchooseto“acti-
516
vate”–whichbydefaultwillbeyourlastsubmission,evenifthatisafterthedeadlineandyoumadesubmissions
517
before the deadline. Any submissions after the deadline8 will incur a late penalty – see the CSSE2310/7231
518
course profile for details.
519
3219
Marks
520
Markswillbeawardedforfunctionality,styleanddocumentation. Marksmaybereducedifyouattendaninter-
521
viewaboutyourassignmentandyouareunabletoadequatelyrespondtoquestions–seetheCSSE7231Student
522
Interviews section below.
523
Functionality (60 marks)
524
Provided your code compiles (see above) and does not use any prohibited statements/functions (see above),
525
and your zip file has been generated correctly and has not been modified before submission, then you will earn
526
functionality marks based on the number of features your program correctly implements, as outlined below.
527
Not all features are of equal difficulty.
528
Partial marks will be awarded for 321p9 artially meeting the functionality requirements. Several tests will be
529
run for each marking category listed below, testing a variety of scenarios. Your mark in each category will be
530
proportional (or approximately proportional) to the number of tests passed in that category.
531
If your program does not allow a feature to be tested, then you will receive 0 marks for that
532
feature, even if you claim to have implemented it. For example, if your program is unable to open uqmakefiles
533
listed on the command line then you will lose marks for most features as we will use a variety of uqmakefiles
534
in our testing. Your tests must run in a reasonable time frame, which could be as short as a few seconds for
535
usagecheckingtomanytensofsecondswhen valgrindisusedtotestformemoryleaks. Ifyourprogramtakes
536
too long to respond, then it will be terminated and you will earn no marks for the functionality associated with
537
that test.
538
3219
Exact text matching of output messages is used for functionality marking. Strict adherence
539
to this specification and matching the behaviour of the demo program are critical to earn func-
540
tionality marks.
541
The course staff will make no alterations to your code prior to testing (other than to remove code without
542
academic merit).
543
Marks will be assigned in the following categories.
544
1. Program correctly handles invalid command lines (usage errors) (3 marks)
545
2. Program correctly handles file errors: inability to open a uqmakefile (1 mark)
546
3. Program correctly handles invalid variable definitions on the command line (2 marks)
547
4. Program correctly handles invalid uqmakefiles (3 marks)
548
5. Program can correctly simulate building non-existent .o targets from existing listed
549
.c file dependencies (no variable substitutions, no phony targets, no modification time
550
checking required) (3 marks)
551
6. Program can correctly simulate building non-existent executables from listed .o
552
dependencies which may or may not exist (no variable substitutions, no phony targets,
553
no modification time checking required) (3 marks)
554
7. Program can correctly simulate building targets where modification time checking
555
is required (no variable substitutions, no phony targets) (3 marks)
556
8. Program can correctly simulate building targets with variable substitutions
557
(no phony targets, includes modification time checking) (3 marks)
558
9. Program can correctly simulate building targets in all circumstances (including
559
phony targets and phony dependencies) (3 marks)
560
10. Program correctly handles missing dependencies when simulating (3 marks)
561
8oryourextendeddeadlineifyouaregrantedanextension.
© 2026 The University of Queensland 14 Version 1.0
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

11. Program can correctly build non-existent .o targets from existing listed
562
.c file dependencies (no variable substitutions, no phony targets, no modification time
563
| checking | required) |     |     |     |     |     |     |     | (3 marks) |     |
| -------- | --------- | --- | --- | --- | --- | --- | --- | --- | --------- | --- |
564
| 12. Program | can | correctly | build | non-existent |     | executables | from listed | .o  |     |     |
| ----------- | --- | --------- | ----- | ------------ | --- | ----------- | ----------- | --- | --- | --- |
565
dependencies which may or may not exist (no variable substitutions, no phony targets,
566
| no  | modification | time | checking | required) |     |     |     |     | (3 marks) |     |
| --- | ------------ | ---- | -------- | --------- | --- | --- | --- | --- | --------- | --- |
567
| 13. Program | can | correctly | build | targets | where | modification | time | checking |     |     |
| ----------- | --- | --------- | ----- | ------- | ----- | ------------ | ---- | -------- | --- | --- |
568
is required (no variable substitutions, no phony targets) (3 marks)
569
| 14. Program | can | correctly | build | targets | with | variable | substitutions |     |     |     |
| ----------- | --- | --------- | ----- | ------- | ---- | -------- | ------------- | --- | --- | --- |
570
(no phony targets, includes modification time checking) (3 marks)
571
15. Program correctly handles compilation and linking failures (includes missing dependencies) (3 marks)
572
| 16. Program | can | correctly | build | targets | in all | circumstances | (including |     |     |     |
| ----------- | --- | --------- | ----- | ------- | ------ | ------------- | ---------- | --- | --- | --- |
573
phony targets and phony dependencies but no pipes in command pipelines) (3 marks)
574
| 17. Program | can | correctly | build | targets | in all | circumstances | (including |     |     |     |
| ----------- | --- | --------- | ----- | ------- | ------ | ------------- | ---------- | --- | --- | --- |
575
phony targets and phony dependencies with pipes in command pipelines) (3 marks)
576
18. Program correctly handles invalid command pipelines and command pipeline failures (3 marks)
577
19. Program correctly implements interruption (signal handling) for a variety of
578
| execution | scenarios |     |     |     |     |     |     |     | (3 marks) |     |
| --------- | --------- | --- | --- | --- | --- | --- | --- | --- | --------- | --- |
579
20. Program correctly closes unnecessary open file descriptors on child processes
580
(for a variety of execution scenarios, including command pipelines) (3 marks)
581
21. Program operates correctly and frees all memory upon exit (for a variety of scenarios
582
| tested | above) |     |     |     |     |     |     |     | (3 marks) |     |
| ------ | ------ | --- | --- | --- | --- | --- | --- | --- | --------- | --- |
583
Some functionality may be assessed in multiple categories. For example, if your program can’t correctly
584
| build object | files | from .c | files then | it will | fail tests | in many | categories. |     |     |     |
| ------------ | ----- | ------- | ---------- | ------- | ---------- | ------- | ----------- | --- | --- | --- |
585
3219
| Style | Marking | (10 | marks) |     |     |     |     |     |     |     |
| ----- | ------- | --- | ------ | --- | --- | --- | --- | --- | --- | --- |
586
This section is unchanged from assignment one. You should still make sure that you are familiar with all of the
| requirements | below. |     |     |     |     |     |     |     |     | 587 |
| ------------ | ------ | --- | --- | --- | --- | --- | --- | --- | --- | --- |
Your style mark is based on the number of style guide violations in your code, i.e. the number of violations
588
of version 4 of the CSSE2310/CSSE7231 C Programming Style Guide (found on Blackboard) along with the
589
numberofwarningsproducedwhencompilingyourcode. Yourstylemarkisautomaticallydeterminedandcan
590
never be more than your functionality mark – this prevents the submission of well styled programs which don’t
591
| meet at | least a minimum |     | level of | required | functionality. |     |     |     |     |     |
| ------- | --------------- | --- | -------- | -------- | -------------- | --- | --- | --- | --- | --- |
592
You are encouraged to use the 2310reformat.sh, 2310stylecheck.sh and 2310checkcommit.sh tools
593
installed on moss to correct and/or check your code style before each commit. The 2310stylecheck.sh tool
594
will determine your automated style mark (see below), but not all requirements in the guide are checked. For
595
example, the tool does not check that names and comments are meaningful. You are still encouraged to make
596
namesandcommentsmeaningfulsinceyourcodeistheonlyresourceyouwillaccesstowhenbeinginterviewed
597
| about | your submission. |     |     |     |     |     |     |     |     |     |
| ----- | ---------------- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
598
All .c and .h source files in your submission will be subject to style marking. This applies whether they
599
| are compiled/linked |     | into | your executable |     | or not9. |     |     |     |     |     |
| ------------------- | --- | ---- | --------------- | --- | -------- | --- | --- | --- | --- | --- |
600
Automated style marks will be calculated over all of your .c and .h files as follows.
601
| 1. If | any of the | following | are | true, | then your | style mark | will be | zero: |     |     |
| ----- | ---------- | --------- | --- | ----- | --------- | ---------- | ------- | ----- | --- | --- |
602
• One or more of your source files does not compile by itself (i.e., the compiler reports an error)10.
603
• One or more of your source files does not start with a correct doxygen @file (or \file) comment.
604
|     | (See the | style | guide for | the | precise requirements.) |     |     |     |     |     |
| --- | -------- | ----- | --------- | --- | ---------------------- | --- | --- | --- | --- | --- |
605
9Makesureyouremoveanyunneededfilesfromyourrepository,ortheywillbesubjecttostylemarking.
10Notethatevery.hfileinyoursubmissionmustmakesensewithoutreferencetoanyotherfiles,e.g.,itmust#includeany.h
filesthatcontaindeclarationsordefinitionsusedinthat.hfile. Youcancheckthataheaderfilecompilesbyitselfbyrunninggcc
| -c filename.h | –withanyothergccargumentsasrequired. |     |            |     |     |     |     |     |             |     |
| ------------- | ------------------------------------ | --- | ---------- | --- | --- | --- | --- | --- | ----------- | --- |
| © 2026        | The University                       | of  | Queensland |     |     | 15  |     |     | Version 1.0 |     |
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

• Your code uses comments/directives to control the behaviour of clang-format or clang-tidy, e.g.
606
by preventing warnings.
607
• Any of your source files contain C functions longer than 100 lines of code11.
608
• Your code contains any global variable definitions (other than a single flag of bool type for signal
609
handling).
610
• 2310stylecheck.sh reports an error of any kind. (This should usually only happen because of a
611
compilation failure or a missing doxygen file comment.)
612
• Your functionality mark is zero (including if this is because of the use of forbidden functions).
613
2. If none of the above are true, then your style mark will be determined as follows.
614
• Consider W to be the total number of warnings produced when running 2310stylecheck.sh – this
615
includes the number of warnings produced when each source file is compiled by itself.
616
• If W ≥ 10, then your style mark S will zero. Otherwise, your style mark S will be
617
S =10−W
618
(No negative marks will be awarded.)
619
If you believe that 2310stylecheck.sh is behaving incorrectly or inconsistently then please bring this to
620
the attention of the teaching staff prior to submission, e.g., it is possible the style checker may report different
621
issuesonmossthanitdoesintheGradescopeenvironment. Yourstylemarkcanbeupdatedifthisisdeemedto
622
be appropriate. You can check the result of Gradescope style marking soon after your Gradescope submission
623
– when the test suite completes running.
624
SVN Commit History Marking (5 marks)
625
This section is unchanged from assignment one. You should still make sure that you are familiar with all of the
requirements below. 626
You are expected to make appropriate use of your SVN repository in the development of your program.
627
Progressive development is expected, i.e., many commits and no large commits with multiple features in them.
628
Every revision is expected to be correctly styled (i.e., 2310stylecheck.sh reports no errors or warn-
629
ings on the files in your repository for that revision). You are encouraged to run 2310checkcommit.sh
630
before each commit so that you can fix styling errors before making the commit12. This tool will run
631
2310stylecheck.sh on the files that are expected to be in your repository after the next commit – based on
632
the current status of your SVN working copy.
633
You should also make appropriate use of log messages to capture the changes represented by each commit.
634
The wording of these messages will not be marked but some minimum standards are expected, e.g. in the
635
number of words used in each message. You are still encouraged to write meaningful messages so that you can
636
easily find a version to revert to if necessary during your development. Meaningful messages explain briefly
637
what has changed in the commit (e.g. in terms of functionality, not in terms of specific numbered test cases
638
in the test suite) and/or why the change has been made and will be usually be more detailed for significant
639
changes.
640
Your SVN Commit History Mark is determined automatically by looking at the trunk/a2 area of your
641
course SVN repository. The starting mark is the number of commits you make (other than the last) that are
642
perfectly styled, i.e., 2310stylecheck.sh does not report any errors or warnings on the source files in your
643
repository at that revision. If this number is greater than 5, then it is capped at 5. One mark will then be
644
removed from your SVN commit history mark for
645
• eachrevision(otherthanthelast)thathasanystyleviolationsreportedwhenrunning2310stylecheck.sh
646
on the source files in that revision. (We don’t check for style violations in the last revision because that
647
styling determines your style mark out of 10 as described above.);
648
• eachcommitthataddsmorethan150linesofcodeoverthepreviouscommit(wherelinesofcodeexclude
649
blank lines and comments);
650
11Notethatthestyleguiderequiresfunctionstobe50linesofcodeorfewer. Codethatcontainsfunctionswhoselengthis51to
100lineswillbepenalisedsomewhat–onestyleviolation(i.e. onemark)perfunction. Codethatcontainsfunctionslongerthan
100lineswillbepenalisedveryheavily–nomarkswillbeawardedforstyle.
12Notethat2310checkcommit.shonlychecksforstyleviolationsandthenumberoflinesofcodeadded. Itdoesnotcheckwhether
yourfuturecommitmessageisappropriate.
© 2026 The University of Queensland 16 Version 1.0
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

| • each | duplicated |     | commit | message; |     |     |     |     |     |     |     |
| ------ | ---------- | --- | ------ | -------- | --- | --- | --- | --- | --- | --- | --- |
651
• each commit message that is shorter than 3 words or longer than 1000 words; and
652
• having an average commit message length of less than 6 words (across all commits to the trunk/a2 area
653
| of  | your repository). |     |     |     |     |     |     |     |     |     |     |
| --- | ----------------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
654
(NotethatinordertogetfullmarksforyourSVNcommithistory,youwillneedtomakeatleast5perfectly
655
| styled commits |     | plus | your last | commit, | i.e. | at least | 6 commits | total.) |     |     |     |
| -------------- | --- | ---- | --------- | ------- | ---- | -------- | --------- | ------- | --- | --- | --- |
656
| Your | SVN | commit | history | mark | C will | therefore | be  |     |     |     |     |
| ---- | --- | ------ | ------- | ---- | ------ | --------- | --- | --- | --- | --- | --- |
657
|     |     |     |     | C =min{5,P}−max{V |     |     | −1,0}−L−D−M |     |     | −A  |     |
| --- | --- | --- | --- | ----------------- | --- | --- | ----------- | --- | --- | --- | --- |
658
| or zero, | if C is | negative, | where |     |     |     |     |     |     |     |     |
| -------- | ------- | --------- | ----- | --- | --- | --- | --- | --- | --- | --- | --- |
659
• P is the number of perfectly styled revisions (other than the last revision) that you make to your
660
repository, i.e., the number that do not contain style guide violations (errors or warnings reported by
661
2310stylecheck.sh);
662
• V is the number of revisions (other than the last revision) that contain any style guide violations (errors
663
| or  | warnings | reported |     | by 2310stylecheck.sh); |     |     |     |     |     |     |     |
| --- | -------- | -------- | --- | ---------------------- | --- | --- | --- | --- | --- | --- | --- |
664
• Listhenumberofcommitsthataddover150linesofcodetoyourrepository(comparedwiththenumber
665
| of  | lines | of code | at your | previous | commit); |     |     |     |     |     |     |
| --- | ----- | ------- | ------- | -------- | -------- | --- | --- | --- | --- | --- | --- |
666
| • D | is the | number | of duplicated |     | commit | messages; |     |     |     |     |     |
| --- | ------ | ------ | ------------- | --- | ------ | --------- | --- | --- | --- | --- | --- |
667
• M is the number of commit messages that are shorter than 3 words or longer than 1000 words; and
668
• is 1 if your average commit message length is less than 6 words and zero otherwise.
A 669
As for assignment one, you won’t be penalised if you have one commit containing style guide violations –
670
| hence the | max{V | −1,0} | term | above. |     |     |     |     |     |     |     |
| --------- | ----- | ----- | ---- | ------ | --- | --- | --- | --- | --- | --- | --- |
671
Total Mark
672
This section has changed since assignment one. Your functionality mark will be capped at 30 out of 60 if you
fail to include at least file level comments in every source file that describe your AI use (or not) in the required
673
Doxygen format. Misisng comments will be reported by 2310stylecheck.sh.
Let
674
• be the functionality mark for your assignment (out of 60, but capped at 30 if you fail to correctly
F 675
| document |     | AI use | (or | not) in | the file | comment | of every | source | file); |     |     |
| -------- | --- | ------ | --- | ------- | -------- | ------- | -------- | ------ | ------ | --- | --- |
676
| • S | be the | style | mark for | your | assignment | (out | of 10); |     |     |     |     |
| --- | ------ | ----- | -------- | ---- | ---------- | ---- | ------- | --- | --- | --- | --- |
677
| • C | be the | SVN | commit | history | mark | (out of | 5); and |     |     |     |     |
| --- | ------ | --- | ------ | ------- | ---- | ------- | ------- | --- | --- | --- | --- |
678
• be the scaling factor (0 to 1) determined after interview (see the CSSE7231 Student Interviews sec-
I 679
tion below) – or 0 if you fail to attend a scheduled interview without having evidence of exceptional
680
circumstances impacting your ability to attend – or 1 if you not required to attend an interview for this
681
assignment.
682
| Your total | mark | for | the assignment |     | will be: |     |     |     |     |     |     |
| ---------- | ---- | --- | -------------- | --- | -------- | --- | --- | --- | --- | --- | --- |
683
|     |     |     |     |     | =(F | + min{F,S} | +   | min{F,C})×I |     |     |     |
| --- | --- | --- | --- | --- | --- | ---------- | --- | ----------- | --- | --- | --- |
M
684
out of 75.
685
In other words, you can’t get more marks for style than you do for functionality. Similarly, you can’t get
686
more marks for SVN commit history than you do for functionality. Pretty code that doesn’t work will not be
687
rewarded!
688
Late Penalties
689
| Late penalties |     | will apply | as  | outlined | in the | course | profile. |     |     |     |     |
| -------------- | --- | ---------- | --- | -------- | ------ | ------ | -------- | --- | --- | --- | --- |
690
| © 2026 The | University |     | of Queensland |     |     |     | 17  |     |     |     | Version 1.0 |
| ---------- | ---------- | --- | ------------- | --- | --- | --- | --- | --- | --- | --- | ----------- |
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02

CSSE7231 Student Interviews
691
A subset of students will be interviewed by teaching staff about their assignment submission.
692
Interviewswilltakeplaceintheweekfollowingyoursubmission(orpossiblyinweek12ofsemesterforthose
693
whomakealatesubmission). Theinterviewmaybeinyourpracsessionorseparatelyorganisedwithamember
694
of the teaching staff. You will be invited (by email to your student email account) to sign up for a particular
695
interview time slot.
696
Students will be selected for interview based on a number of factors that may include (but are not limited
697
to):
698
• Feedback from course staff based on observations in class, on the discussion forum, and during marking;
699
• An unusual commit history (versions and/or messages), e.g. limited evidence of progressive development;
700
• Variation of student performance, code style, etc. over time;
701
• Use of unusual or uncommon code structure/functions etc.;
702
• Referencing, or lack of referencing, present in code;
703
• Use of, or suspicion of undocumented use of, artificial intelligence or other code generation tools; and
704
• Reports from students or others about student work.
705
The rest of the text in this section is unchanged from assignment one.
706
Theinterviewisforthepurposesofestablishingyourunderstandingofthecodeyousubmitted. Ifyouwrite
707
your own code, you have nothing to fear from this process. If you legitimately use code from other sources
708
(following the usage/referencing requirements outlined in this assignment and the style guide) then you are
709
expected to understand that code. If you are not able to adequately explain the design of your solution and/or
710
adequatelyexplainyour submitted code (including theuseandbehaviourof library functions andsystemcalls)
711
and/or be able to make or describe simple modifications to your code as requested at the interview, then your
712
assignment mark will be scaled down based on the level of understanding you are able to demonstrate. If it
713
becomes clear that you did not write code that you claim is your own or you have not referenced the source
714
of any of your code correctly then your submission may be subject to a misconduct investigation where your
715
interview responses form part of the evidence.
716
Interviews will usually take about 10 minutes, but some may be shorter (e.g., if your level of understanding
717
becomesapparentquickly)andsomemaybelonger. You must bring your own laptop to your interview
718
and be ready to show your submitted code (e.g., via Gradescope or using vim on moss). You
719
will be required to join a Zoom meeting so your screen and the interview is recorded. You must
720
bring your UQ student card or government ID to your interview so that your identity can be
721
confirmed.
722
You may not access any notes or other resources (such as websites, AI tools and man pages) during the
723
interview–youmayonlylookatthecodeyousubmitted(whichmustnotbemodifiedaftersubmission). Youare
724
encouraged to appropriately comment your code before submission to refresh or aid your understanding when
725
you are interviewed about the code. Machine translation tools are permitted to be used during the interview
726
butquestionswillbeinEnglishandallanswersmustbeinEnglish. Takingtoomuchtimetoansweraquestion
727
may be interpreted as a lack of understanding of the topic of the question and may result in reduced marks.
728
Scaling to zero marks is possible if no or very limited understanding is demonstrated.
729
Failuretoattendanintervieworfailuretorespondtoaninterviewinvitationwillresultinzeromarksforthe
730
assignment unless there are documented exceptional circumstances that prevent you from attending, in which
731
case a single reschedule of the interview will be permitted.
732
Specification Updates
733
Any errors discovered in the assignment specification will be corrected and new versions will be released with
734
adequate time for students to respond before the due date. Potential specification errors or inconsistencies
735
can be discussed on the discussion forum. If your program3219 ’s expected behaviour is not described in this
736
specification, then you can use the demo program (demo-uqmakelite) to determine the expected behaviour.
737
We will not update this specification when the expected behaviour is missing from this specification, only
738
when this specification is inconsistent with itself or with the behaviour of the demo program or where it is not
739
reasonably possible to determine the expected behaviour from the demo program.
740
© 2026 The University of Queensland 18 Version 1.0
DocumentcreatedforFrankWU(s4893827)only. Documentgenerated2026-04-1516:02