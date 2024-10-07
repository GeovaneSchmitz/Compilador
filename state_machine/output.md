# State Machine

## State: 0 (Initial)
- **Transition on:** `*`, `/`, `%`  
  **Next State:** 4 (Final, id: High priority operator)
- **Transition on:** `,`  
  **Next State:** 17 (Final, id: Comma)
- **Transition on:** `)`  
  **Next State:** 16 (Final, id: Close parenthesis)
- **Transition on:** `(`  
  **Next State:** 15 (Final, id: Open parenthesis)
- **Transition on:** `!`  
  **Next State:** 14
- **Transition on:** `>`  
  **Next State:** 13 (Final, id: Comparator)
- **Transition on:** `[`  
  **Next State:** 12 (Final, id: Open square bracket)
- **Transition on:** `"`  
  **Next State:** 1
- **Transition on:** `<`  
  **Next State:** 8
- **Transition on:** `;`  
  **Next State:** 2 (Final, id: Semicolon)
- **Transition on:** `]`  
  **Next State:** 3 (Final, id: Close square bracket)
- **Transition on:** `Letter`  
  **Next State:** 5
- **Transition on:** `+`, `-`  
  **Next State:** 6 (Final, id: Low priority operator)
- **Transition on:** `{`  
  **Next State:** 7 (Final, id: Open curly brace)
- **Transition on:** `}`  
  **Next State:** 10 (Final, id: Close curly brace)
- **Transition on:** `Digit`  
  **Next State:** 9
- **Transition on:** `=`  
  **Next State:** 11 (Final, id: Assignment)

---

## State: 1
- **Transition on:** `"`  
  **Next State:** 29 (Final, id: String constant)
- **Transition on:** `Alphanumeric`  
  **Next State:** 1 (Loop)

---

## State: 2 (Final)
Final id: Semicolon  
No transitions

---

## State: 3 (Final)
Final id: Close square bracket  
No transitions

---

## State: 4 (Final)
Final id: High priority operator  
No transitions

---

## State: 5
- **Transition on:** `Default`  
  **Next State:** 28 (Rollback Final, id: Ident)
- **Transition on:** `Alphanumeric`  
  **Next State:** 5 (Loop)

---

## State: 6 (Final)
Final id: Low priority operator  
No transitions

---

## State: 7 (Final)
Final id: Open curly brace  
No transitions

---

## State: 8
- **Transition on:** `Default`  
  **Next State:** 27 (Rollback Final, id: Comparator)
- **Transition on:** `=`  
  **Next State:** 26 (Final, id: Comparator)

---

## State: 9
- **Transition on:** `.`  
  **Next State:** 23
- **Transition on:** `Digit`  
  **Next State:** 9 (Loop)
- **Transition on:** `Default`  
  **Next State:** 22 (Rollback Final, id: Int constant)

---

## State: 10 (Final)
Final id: Close curly brace  
No transitions

---

## State: 11 (Final)
Final id: Assignment  
- **Transition on:** `=`  
  **Next State:** 21 (Final, id: Comparator)

---

## State: 12 (Final)
Final id: Open square bracket  
No transitions

---

## State: 13 (Final)
Final id: Comparator  
- **Transition on:** `Default`  
  **Next State:** 20 (Rollback Final, id: Comparator)
- **Transition on:** `=`  
  **Next State:** 19

---

## State: 14
- **Transition on:** `=`  
  **Next State:** 18 (Final, id: Comparator)

---

## State: 15 (Final)
Final id: Open parenthesis  
No transitions

---

## State: 16 (Final)
Final id: Close parenthesis  
No transitions

---

## State: 17 (Final)
Final id: Comma  
No transitions

---

## State: 18 (Final)
Final id: Comparator  
No transitions

---

## State: 19
No transitions

---

## State: 20 (Rollback Final)
Final id: Comparator  
No transitions

---

## State: 21 (Final)
Final id: Comparator  
No transitions

---

## State: 22 (Rollback Final)
Final id: Int constant  
No transitions

---

## State: 23
- **Transition on:** `Digit`  
  **Next State:** 24

---

## State: 24
- **Transition on:** `Default`  
  **Next State:** 25 (Rollback Final, id: Float constant)
- **Transition on:** `Digit`  
  **Next State:** 24 (Loop)

---

## State: 25 (Rollback Final)
Final id: Float constant  
No transitions

---

## State: 26 (Final)
Final id: Comparator  
No transitions

---

## State: 27 (Rollback Final)
Final id: Comparator  
No transitions

---

## State: 28 (Rollback Final)
Final id: Ident  
No transitions

---

## State: 29 (Final)
Final id: String constant  
No transitions
