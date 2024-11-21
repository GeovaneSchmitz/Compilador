## State: 0 { Initial }
### Transitions:
- **s** -> State 26
- **r** -> State 25
- **p** -> State 23
- **[** -> State 22
- **n** -> State 21
- **=** -> State 9
- **e** -> State 20
- **f** -> State 15
- **+ -** -> State 7
- **,** -> State 10
- **>** -> State 6
- **!** -> State 4
- **(** -> State 3
- **A-Z a c g-h j-m o q t-z** -> State 13
- **)** -> State 18
- **<** -> State 5
- **d** -> State 19
- **0-9** -> State 14
- **"** -> State 1
- **% * /** -> State 8
- **;** -> State 12
- **{** -> State 11
- **]** -> State 24
- **}** -> State 2
- **i** -> State 17
- **b** -> State 16

## State: 1 
### Transitions:
- **"** -> State 74
- **0-9 A-Z a-z** -> State 1

## State: 2 { Final }
- **Final ID**: Close curly brace 

## State: 3 { Final }
- **Final ID**: Open parenthesis 

## State: 4 
### Transitions:
- **=** -> State 73

## State: 5 
### Transitions:
- **=** -> State 71
- **Default** -> State 72

## State: 6 { Final }
- **Final ID**: Comparator 
### Transitions:
- **=** -> State 69
- **Default** -> State 70

## State: 7 { Final }
- **Final ID**: Low priority operator 

## State: 8 { Final }
- **Final ID**: High priority operator 

## State: 9 { Final }
- **Final ID**: Assignment 
### Transitions:
- **=** -> State 68

## State: 10 { Final }
- **Final ID**: Comma 

## State: 11 { Final }
- **Final ID**: Open curly brace 

## State: 12 { Final }
- **Final ID**: Semicolon 

## State: 13 
### Transitions:
- **Default** -> State 27
- **0-9 A-Z a-z** -> State 13

## State: 14 
### Transitions:
- **.** -> State 64
- **0-9** -> State 14
- **Default** -> State 65

## State: 15 
### Transitions:
- **Default** -> State 27
- **o** -> State 58
- **l** -> State 59
- **0-9 A-Z a-k m-n p-z** -> State 13

## State: 16 
### Transitions:
- **Default** -> State 27
- **r** -> State 54
- **0-9 A-Z a-q s-z** -> State 13

## State: 17 
### Transitions:
- **Default** -> State 27
- **f** -> State 51
- **n** -> State 52
- **0-9 A-Z a-e g-m o-z** -> State 13

## State: 18 { Final }
- **Final ID**: Close parenthesis 

## State: 19 
### Transitions:
- **Default** -> State 27
- **e** -> State 49
- **0-9 A-Z a-d f-z** -> State 13

## State: 20 
### Transitions:
- **Default** -> State 27
- **l** -> State 46
- **0-9 A-Z a-k m-z** -> State 13

## State: 21 
### Transitions:
- **Default** -> State 27
- **e** -> State 44
- **0-9 A-Z a-d f-z** -> State 13

## State: 22 { Final }
- **Final ID**: Open square bracket 

## State: 23 
### Transitions:
- **Default** -> State 27
- **r** -> State 40
- **0-9 A-Z a-q s-z** -> State 13

## State: 24 { Final }
- **Final ID**: Close square bracket 

## State: 25 
### Transitions:
- **Default** -> State 27
- **e** -> State 33
- **0-9 A-Z a-d f-z** -> State 13

## State: 26 
### Transitions:
- **t** -> State 28
- **Default** -> State 27
- **0-9 A-Z a-s u-z** -> State 13

## State: 27 { Rollback Final }
- **Final ID**: Ident 

## State: 28 
### Transitions:
- **Default** -> State 27
- **r** -> State 29
- **0-9 A-Z a-q s-z** -> State 13

## State: 29 
### Transitions:
- **Default** -> State 27
- **i** -> State 30
- **0-9 A-Z a-h j-z** -> State 13

## State: 30 
### Transitions:
- **Default** -> State 27
- **n** -> State 31
- **0-9 A-Z a-m o-z** -> State 13

## State: 31 
### Transitions:
- **Default** -> State 27
- **0-9 A-Z a-f h-z** -> State 13
- **g** -> State 32

## State: 32 { Rollback Final }
- **Final ID**: Reserved word string 
### Transitions:
- **Default** -> State 27
- **0-9 A-Z a-z** -> State 13

## State: 33 
### Transitions:
- **Default** -> State 27
- **t** -> State 34
- **a** -> State 35
- **0-9 A-Z b-s u-z** -> State 13

## State: 34 
### Transitions:
- **Default** -> State 27
- **u** -> State 37
- **0-9 A-Z a-t v-z** -> State 13

## State: 35 
### Transitions:
- **Default** -> State 27
- **d** -> State 36
- **0-9 A-Z a-c e-z** -> State 13

## State: 36 { Rollback Final }
- **Final ID**: Reserved word read 
### Transitions:
- **Default** -> State 27
- **0-9 A-Z a-z** -> State 13

## State: 37 
### Transitions:
- **Default** -> State 27
- **r** -> State 38
- **0-9 A-Z a-q s-z** -> State 13

## State: 38 
### Transitions:
- **Default** -> State 27
- **n** -> State 39
- **0-9 A-Z a-m o-z** -> State 13

## State: 39 { Rollback Final }
- **Final ID**: Reserved word return 
### Transitions:
- **Default** -> State 27
- **0-9 A-Z a-z** -> State 13

## State: 40 
### Transitions:
- **Default** -> State 27
- **i** -> State 41
- **0-9 A-Z a-h j-z** -> State 13

## State: 41 
### Transitions:
- **Default** -> State 27
- **n** -> State 42
- **0-9 A-Z a-m o-z** -> State 13

## State: 42 
### Transitions:
- **Default** -> State 27
- **t** -> State 43
- **0-9 A-Z a-s u-z** -> State 13

## State: 43 { Rollback Final }
- **Final ID**: Reserved word print 
### Transitions:
- **Default** -> State 27
- **0-9 A-Z a-z** -> State 13

## State: 44 
### Transitions:
- **Default** -> State 27
- **w** -> State 45
- **0-9 A-Z a-v x-z** -> State 13

## State: 45 { Rollback Final }
- **Final ID**: Reserved word new 
### Transitions:
- **Default** -> State 27
- **0-9 A-Z a-z** -> State 13

## State: 46 
### Transitions:
- **Default** -> State 27
- **s** -> State 47
- **0-9 A-Z a-r t-z** -> State 13

## State: 47 
### Transitions:
- **Default** -> State 27
- **e** -> State 48
- **0-9 A-Z a-d f-z** -> State 13

## State: 48 { Rollback Final }
- **Final ID**: Reserved word else 
### Transitions:
- **Default** -> State 27
- **0-9 A-Z a-z** -> State 13

## State: 49 
### Transitions:
- **Default** -> State 27
- **f** -> State 50
- **0-9 A-Z a-e g-z** -> State 13

## State: 50 { Rollback Final }
- **Final ID**: Reserved word def 
### Transitions:
- **Default** -> State 27
- **0-9 A-Z a-z** -> State 13

## State: 51 { Rollback Final }
- **Final ID**: Reserved word if 
### Transitions:
- **Default** -> State 27
- **0-9 A-Z a-z** -> State 13

## State: 52 
### Transitions:
- **t** -> State 53
- **Default** -> State 27
- **0-9 A-Z a-s u-z** -> State 13

## State: 53 { Rollback Final }
- **Final ID**: Reserved word int 
### Transitions:
- **Default** -> State 27
- **0-9 A-Z a-z** -> State 13

## State: 54 
### Transitions:
- **Default** -> State 27
- **e** -> State 55
- **0-9 A-Z a-d f-z** -> State 13

## State: 55 
### Transitions:
- **Default** -> State 27
- **a** -> State 56
- **0-9 A-Z b-z** -> State 13

## State: 56 
### Transitions:
- **Default** -> State 27
- **k** -> State 57
- **0-9 A-Z a-j l-z** -> State 13

## State: 57 { Rollback Final }
- **Final ID**: Reserved word break 
### Transitions:
- **Default** -> State 27
- **0-9 A-Z a-z** -> State 13

## State: 58 
### Transitions:
- **Default** -> State 27
- **r** -> State 63
- **0-9 A-Z a-q s-z** -> State 13

## State: 59 
### Transitions:
- **Default** -> State 27
- **o** -> State 60
- **0-9 A-Z a-n p-z** -> State 13

## State: 60 
### Transitions:
- **a** -> State 61
- **Default** -> State 27
- **0-9 A-Z b-z** -> State 13

## State: 61 
### Transitions:
- **t** -> State 62
- **Default** -> State 27
- **0-9 A-Z a-s u-z** -> State 13

## State: 62 { Rollback Final }
- **Final ID**: Reserved word float 
### Transitions:
- **Default** -> State 27
- **0-9 A-Z a-z** -> State 13

## State: 63 { Rollback Final }
- **Final ID**: Reserved word for 
### Transitions:
- **Default** -> State 27
- **0-9 A-Z a-z** -> State 13

## State: 64 
### Transitions:
- **0-9** -> State 66

## State: 65 { Rollback Final }
- **Final ID**: Int constant 

## State: 66 
### Transitions:
- **0-9** -> State 66
- **Default** -> State 67

## State: 67 { Rollback Final }
- **Final ID**: Float constant 

## State: 68 { Final }
- **Final ID**: Comparator 

## State: 69 

## State: 70 { Rollback Final }
- **Final ID**: Comparator 

## State: 71 { Final }
- **Final ID**: Comparator 

## State: 72 { Rollback Final }
- **Final ID**: Comparator 

## State: 73 { Final }
- **Final ID**: Comparator 

## State: 74 { Final }
- **Final ID**: String constant 

