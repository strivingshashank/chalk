/**
 * @file parser.h
 * @brief Parser API: recursive descent parser, AST definition, evaluator.
 */


#ifndef PARSER_H
#define PARSER_H

#include "../include/lexer.h"

New chatSearchCustomizeChatsProjectsArtifactsCodeRecentsHideVector insert implementation bugTest cases for lexer and AST dumpExisting languages for your projectGrammar for a simple print expression languageUntitledUntitledExpression grammar rulesSymbol table implementation in C compilersLexer issues with nested brackets and powersMath-focused scripting language designFlex and bison for VM developmentRecursive descent parser implementationBuilding a custom kernel and operating systemBuilding a math expression compilerLearning AI agents for marketingLearning system programming with C and LinuxFinding skincare startups in India by company sizeSShashankFree planVector insert implementation bugvoid *vector_insert(vector_t *vector, size_t idx, const void *data_in) {
        if (!vector || !data_in) {
return NULL;
        }
if (idx > vector->size) {
return NULL;
        }
if (vector->capacity <= vector->size) {
#define VECTOR_GROWTH_FACTOR 2
size_t new_capacity = (vector->capacity == 0) ? 4 : vector->capacity * 2;
char *new_data = (char *)realloc(vector->data, new_capacity);
                /* return early if realloc fails */
if (!new_data) {
return NULL;
                }
vector->data = new_data;
#undef VECTOR_GROWTH_FACTOR
        }
vector->size++; 
if (idx == vector->size) {
return memcpy((vector->data + (idx * vector->data_size)), data_in, vector->data_size);
        }
return memmove((vector->data + (idx * vector->data_size) + 1, (vector->data + (idx * vector->data_size), vector->data_size);
}Show more10:38 AMThere are several bugs in this code. Here's a breakdown:
Bug 1: realloc missing element size multiplier
c// Buggy
char *new_data = (char *)realloc(vector->data, new_capacity);

// Fixed
char *new_data = (char *)realloc(vector->data, new_capacity * vector->data_size);
realloc needs the total byte size, not just the number of elements.

Bug 2: Premature size++ breaks the end-of-vector check
c// Buggy
vector->size++;
if (idx == vector->size) { // idx can never equal size after incrementing (off-by-one)

// Fixed
if (idx == vector->size) { // check BEFORE incrementing
    vector->size++;
    return memcpy(...);
}
vector->size++;
After size++, idx == vector->size can only be true if idx was already out of bounds. The append-to-end case is never reached correctly.

Bug 3: Mismatched parentheses in memmove call
c// Buggy — mismatched parens, wrong arguments entirely
return memmove((vector->data + (idx * vector->data_size) + 1,
               (vector->data + (idx * vector->data_size),
               vector->data_size);

// Fixed
return memmove(vector->data + ((idx + 1) * vector->data_size),
               vector->data + (idx * vector->data_size),
               (vector->size - idx) * vector->data_size);
Three issues here:

Extra unmatched opening parentheses
The destination pointer adds +1 byte instead of +1 element
The size argument only moves data_size bytes instead of shifting all elements from idx onward


Here's the corrected function in full:
cvoid *vector_insert(vector_t *vector, size_t idx, const void *data_in) {
    if (!vector || !data_in) {
        return NULL;
    }
    if (idx > vector->size) {
        return NULL;
    }
    if (vector->capacity <= vector->size) {
        size_t new_capacity = (vector->capacity == 0) ? 4 : vector->capacity * 2;
        char *new_data = (char *)realloc(vector->data, new_capacity * vector->data_size); // Bug 1 fixed
        if (!new_data) {
            return NULL;
        }
        vector->data = new_data;
        vector->capacity = new_capacity;
    }
    if (idx == vector->size) {  // Bug 2 fixed: check before increment
        vector->size++;
        return memcpy(vector->data + (idx * vector->data_size), data_in, vector->data_size);
    }
    vector->size++;
    memmove(vector->data + ((idx + 1) * vector->data_size),  // Bug 3 fixed
            vector->data + (idx * vector->data_size),
            (vector->size - idx - 1) * vector->data_size);
    return memcpy(vector->data + (idx * vector->data_size), data_in, vector->data_size);
}

Bonus: capacity was never updated
In the original code (and the snippet above it's now fixed), after a successful realloc, vector->capacity was never set to new_capacity. This would cause the growth branch to re-trigger on every subsequent insert, repeatedly calling realloc unnecessarily.void *vector_insert(vector_t *vector, size_t idx, const void *data_in) {
        if (!vector || !data_in) {
return NULL;
        }
if (idx > vector->size) {
return NULL;
        }
if (vector->capacity <= vector->size) {
#define VECTOR_GROWTH_FACTOR 2
size_t new_capacity = (vector->capacity == 0) ? 4 : vector->capacity * 2;
char *new_data = (char *)realloc(
vector->data, 
                        (new_capacity * vector->data_size)
                );
                /* return early if realloc fails */
if (!new_data) {
return NULL;
                }
vector->data = new_data;
vector->capacity = new_capacity
#undef VECTOR_GROWTH_FACTOR
        }
if (idx == vector->size) {
vector->size++; 
return memcpy(
                        (vector->data + (idx * vector->data_size)),
data_in, vector->data_size
                );
        }
vector->size++; 
return memmove(
vector->data + ((idx + 1) * vector->data_size),
vector->data + (idx * vector->data_size),
                (vector->size - idx) * vector->data_size
        );
}Show more10:41 AMGood progress from the last version — most bugs are fixed, but there are still two remaining issues:

Bug 1: Missing semicolon after vector->capacity = new_capacity
c// Buggy
vector->capacity = new_capacity
#undef VECTOR_GROWTH_FACTOR

// Fixed
vector->capacity = new_capacity;
#undef VECTOR_GROWTH_FACTOR

Bug 2: memmove shifts elements but never writes the new data
c// Buggy — shifts existing elements but returns without inserting data_in
vector->size++;
return memmove(
    vector->data + ((idx + 1) * vector->data_size),
    vector->data + (idx * vector->data_size),
    (vector->size - idx) * vector->data_size
);

// Fixed — shift first, then write the new element into the gap
vector->size++;
memmove(
    vector->data + ((idx + 1) * vector->data_size),
    vector->data + (idx * vector->data_size),
    (vector->size - idx - 1) * vector->data_size
);
return memcpy(
    vector->data + (idx * vector->data_size),
    data_in,
    vector->data_size
);
Two things corrected here:

The memmove size was (vector->size - idx) but since size was already incremented, this copies one element too many. It should be (vector->size - idx - 1) to reflect the count of elements being shifted, not including the new slot.
memmove only makes room — a subsequent memcpy is needed to actually place data_in at idx.


Corrected function in full:
cvoid *vector_insert(vector_t *vector, size_t idx, const void *data_in) {
    if (!vector || !data_in) {
        return NULL;
    }
    if (idx > vector->size) {
        return NULL;
    }
    if (vector->capacity <= vector->size) {
        size_t new_capacity = (vector->capacity == 0) ? 4 : vector->capacity * 2;
        char *new_data = (char *)realloc(
            vector->data,
            (new_capacity * vector->data_size)
        );
        if (!new_data) {
            return NULL;
        }
        vector->data = new_data;
        vector->capacity = new_capacity; // Bug 1 fixed
    }
    if (idx == vector->size) {
        vector->size++;
        return memcpy(
            (vector->data + (idx * vector->data_size)),
            data_in, vector->data_size
        );
    }
    vector->size++;
    memmove(                                           // Bug 2 fixed
        vector->data + ((idx + 1) * vector->data_size),
        vector->data + (idx * vector->data_size),
        (vector->size - idx - 1) * vector->data_size
    );
    return memcpy(
        vector->data + (idx * vector->data_size),
        data_in,
        vector->data_size
    );
}void *vector_insert(vector_t *vector, size_t idx, const void *data_in) {
        if (!vector || !data_in) {
return NULL;
        }
if (idx > vector->size) {
return NULL;
        }
if (vector->capacity <= vector->size) {
#define VECTOR_GROWTH_FACTOR 2
size_t new_capacity = (vector->capacity == 0) ? 4 : vector->capacity * 2;
char *new_data = (char *)realloc(
vector->data, 
                        (new_capacity * vector->data_size)
                );
                /* return early if realloc fails */
if (!new_data) {
return NULL;
                }
vector->data = new_data;
vector->capacity = new_capacity;
#undef VECTOR_GROWTH_FACTOR
        }
if (idx != vector->size) {
memmove(
vector->data + ((idx + 1) * vector->data_size),
vector->data + (idx * vector->data_size),
                        (vector->size - idx) * vector->data_size
                );
        }
vector->size++; 
return memcpy(
                (vector->data + (idx * vector->data_size)),
data_in, vector->data_size
        );
}Show more10:42 AMThis looks correct to me. All the previous bugs are fixed, and the logic is sound:

realloc uses the correct byte size
capacity is updated after realloc
memmove only runs when inserting in the middle, with the shift happening before size++ — so (vector->size - idx) is the correct count of elements to move at that point
memcpy then writes data_in into the vacated slot
The append case (idx == vector->size) skips memmove and falls straight through to memcpy

The restructuring from the previous version (consolidating the two size++ paths and doing memmove before the increment) cleanly resolves the off-by-one in the shift count as well. No issues to report.Will this insert for 0, idx, and last?10:42 AMYes, all three cases work correctly. Tracing through each:
Insert at 0 (front)

idx == 0, idx != vector->size (assuming size > 0), so memmove shifts all existing elements one slot right
memcpy writes data_in into slot 0
Correct

Insert at idx (middle)

idx != vector->size, so memmove shifts elements from idx onward one slot right by (vector->size - idx) elements
memcpy writes data_in into the vacated slot at idx
Correct

Insert at last (append)

idx == vector->size, so memmove is skipped
memcpy writes data_in into the next available slot
Correct

Edge case: insert at 0 into an empty vector

idx == 0, vector->size == 0, so idx == vector->size, memmove is skipped
memcpy writes into slot 0
Correct
Is the idea of returning the inserted element smart? How can I use it later?10:43 AMIt's a reasonable design choice with some useful applications, but it has tradeoffs.
How you can use it:
c// Immediately dereference to confirm what was stored
int *inserted = (int *)vector_insert(vector, idx, &value);
printf("Inserted: %d\n", *inserted);

// Modify the element after insertion
int *inserted = (int *)vector_insert(vector, idx, &value);
*inserted = 42; // update in-place without another lookup

// NULL-check to detect failure in one step
if (!vector_insert(vector, idx, &value)) {
    // handle error
}
The danger — pointer invalidation:
The returned pointer becomes invalid after any subsequent insert or operation that triggers a realloc, since realloc may move the entire buffer to a new address:
cint *inserted = (int *)vector_insert(vector, 0, &value); // get pointer
vector_insert(vector, 0, &other);                        // may realloc!
printf("%d\n", *inserted);                               // undefined behaviour
The alternatives:

Return the index instead — always stable, caller can re-derive the pointer when needed
Return a boolean/error code — simpler, just signals success or failure
Return the pointer but document the invalidation caveat clearly — which is essentially what most C standard library functions do

The current design is consistent with how memcpy and memmove themselves work (they also return the destination pointer), so it feels natural in a C context. Just make sure the invalidation risk is documented.What are your design recommendations for a header-only vector library for C. Later, umap (unordered map) as well.10:44 AMSynthesized practical design guidance for header-only C librariesSynthesized practical design guidance for header-only C librariesGood project. Here are my recommendations organized by concern:

Header-only structure

Use static inline for all functions to avoid multiple definition errors across translation units
Guard with #ifndef VECTOR_H / #define VECTOR_H / #endif
Consider an implementation macro pattern so users can opt into the definitions in one TU, similar to how stb libraries work:

c#ifdef VECTOR_IMPLEMENTATION
// function bodies
#endif
This avoids bloating every translation unit that includes the header.

The struct
ctypedef struct {
    void   *data;
    size_t  size;
    size_t  capacity;
    size_t  data_size;
} vector_t;

Keep it as-is — it's clean
Consider adding an optional void (*destroy)(void *) function pointer for element-level cleanup, useful when elements own heap memory


API surface to implement
Core:
cvector_t vector_create(size_t data_size);
void     vector_destroy(vector_t *);
void    *vector_insert(vector_t *, size_t idx, const void *data_in);
void    *vector_push(vector_t *, const void *data_in);   // append shorthand
bool     vector_remove(vector_t *, size_t idx);
void    *vector_get(vector_t *, size_t idx);
size_t   vector_size(const vector_t *);
bool     vector_empty(const vector_t *);
Nice to have:
cvoid    *vector_pop(vector_t *);                          // remove + return last
bool     vector_reserve(vector_t *, size_t capacity);    // pre-allocate
bool     vector_shrink(vector_t *);                      // shrink to fit
void     vector_clear(vector_t *);                       // reset size to 0
void    *vector_find(vector_t *, const void *target, int (*cmp)(const void *, const void *));
void     vector_sort(vector_t *, int (*cmp)(const void *, const void *));  // wraps qsort

Iteration
Avoid requiring index-based loops. Provide a clean way to iterate:
c// Macro-based foreach
#define VECTOR_FOREACH(vec, T, var) \
    for (T *var = (T *)(vec)->data; \
         var < (T *)((vec)->data) + (vec)->size; \
         var++)
Usage:
cVECTOR_FOREACH(&vec, int, it) {
    printf("%d\n", *it);
}

Error handling

Return NULL for pointer-returning functions on failure (as you have now)
Return bool for void-like operations
Optionally expose a vector_errno or last-error string — useful for debugging but adds complexity, so only worth it if this is a serious library


Growth strategy

Your current factor-of-2 with initial capacity of 4 is fine
Consider making it configurable:

c#ifndef VECTOR_GROWTH_FACTOR
#define VECTOR_GROWTH_FACTOR 2
#endif

#ifndef VECTOR_INITIAL_CAPACITY
#define VECTOR_INITIAL_CAPACITY 4
#endif
This lets users tune for their use case without modifying the library.

Looking ahead to umap
Design the vector well because your umap will likely use it internally — a common approach is:

Array of buckets, where each bucket is a vector_t of key-value pairs (chaining)
Keeping the vector API clean and general now pays off when you build on top of it

For the umap you'll additionally need to think about:

How keys are typed — void * again, with a user-supplied hash and cmp function
Load factor and rehashing threshold (0.75 is conventional)
Whether to support iteration over entries, which is harder with open addressing than chaining


General hygiene

Document pointer invalidation on any function that may realloc
assert preconditions in a debug build (NDEBUG-guarded)
Write a test file alongside the header from day one — it doubles as documentation
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
        char *data;
        size_t data_size;
        size_t size; // TODO: change to count
        size_t capacity;
} vector_t;

vector_t *vector_new(size_t datapastedHow's this?11:12 AMScrutinized code and unearthed multiple critical bugsScrutinized code and unearthed multiple critical bugsGood structure overall, but there are several bugs:

Bug 1: vector_free uses wrong variable name
c// Buggy
if (!vector || !*vector)

// Fixed
if (!vector_addr || !*vector_addr)
vector is not in scope here — the parameter is vector_addr.

Bug 2: vector_at has two errors in the pointer arithmetic
c// Buggy
(void *)vector->data + (idx + data_size);

// Fixed
(void *)(vector->data + (idx * vector->data_size));

data_size is not in scope — should be vector->data_size
+ should be * — you want offset by idx elements, not idx bytes


Bug 3: vector_pop passes a pointer where an index is expected
c// Buggy — vector_back returns void*, vector_remove expects size_t
void vector_pop(vector_t *vector) {
    return vector_remove(vector, vector_back(vector));
}

// Fixed
void vector_pop(vector_t *vector) {
    if (!vector_is_empty(vector)) {
        vector_remove(vector, vector_count(vector) - 1);
    }
}
Also return in a void function is wrong.

Bug 4: vector_remove condition is always true, causing a NULL memmove destination
c// Buggy — idx is always < size here, so this never skips memmove
if (idx != vector_count(vector))

// Fixed
if (idx != vector_count(vector) - 1)
When removing the last element, vector_at(vector, idx + 1) returns NULL since idx + 1 == size. The memmove destination becomes NULL.

Bug 5: vector_insert memmove has the same NULL destination problem
vector_at bounds-checks against size, but size hasn't been incremented yet when memmove is called. So vector_at(vector, idx + 1) returns NULL when idx == size - 1 (inserting before the last element). Use raw arithmetic here instead:
c// Buggy
memmove(
    vector_at(vector, idx + 1),
    vector_at(vector, idx),
    (vector->size - idx) * vector->data_size
);

// Fixed
memmove(
    vector->data + ((idx + 1) * vector->data_size),
    vector->data + (idx * vector->data_size),
    (vector->size - idx) * vector->data_size
);

Bug 6: Missing #endif directives
c#endif /* VECTOR_IMPLEMENT */
#endif /* _VECTOR_H_ */

Minor notes:

The TODO comment about renaming size to count is worth doing — vector_count and vector->size being different names for the same thing is confusing internally
vector_is_empty returning int is fine for C, but bool is already included and more expressive
VECTOR_GROWTH_FACTOR is #defined but never actually used in the calculation — * 2 is hardcoded
excerpt_from_previous_claude_message.txt13 linestxtHow does this make any difference here?11:17 AMAnalyzed bounds-checking behavior versus raw pointer arithmeticAnalyzed bounds-checking behavior versus raw pointer arithmeticThe difference is vector_at's bounds check:
creturn (!vector || (vector->size <= idx)) ? NULL : ...
When inserting before the last element, idx + 1 == vector->size, so vector->size <= idx + 1 is true and vector_at returns NULL — giving memmove a NULL destination before size has been incremented.
Raw arithmetic has no such check, so it correctly computes the address of the slot just past the last valid element, which is legal memory since realloc already ensured the capacity is there.
In short: vector_at is a safe accessor for reading existing elements, but it's too conservative for internal operations that intentionally address one slot beyond the current size.#ifndef VECTOR_H
#define VECTOR_H

#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
        char *data;
        size_t data_size;
        size_t count;
        size_t capacity;
} vector_t;

vector_t *vector_new(size_t data_size);
void vector_free(vecpastedCheck again:11:20 AMScrutinized code for pointer arithmetic and boundary condition vulnerabilitiesScrutinized code for pointer arithmetic and boundary condition vulnerabilitiesOne remaining bug and one minor issue:

Bug: void * pointer arithmetic in vector_at is undefined behavior
c// Buggy — cast to void* happens before +, making arithmetic operate on void*
(void *)vector->data + (idx * vector->data_size)

// Fixed — arithmetic on char* first, then cast the result
(void *)(vector->data + (idx * vector->data_size))
GCC permits void * arithmetic as an extension (treating it like char *), but it's undefined in standard C. Since vector->data is already char *, move the cast outside so the arithmetic is well-defined.

Minor: VECTOR_GROWTH_FACTOR is defined but never used
c// The macro is defined but the calculation hardcodes 2 anyway
size_t new_capacity = (vector->capacity == 0) ? 4 : vector->capacity * VECTOR_GROWTH_FACTOR;
Either use the macro in the calculation or drop it.

Everything else looks correct — vector_free, vector_remove, vector_pop, the memmove logic in both insert and remove, and the bounds checking all look good.excerpt_from_previous_claude_message.txt2 linestxtAlternatively, can I just omit it?11:21 AMEvaluated implicit pointer conversion feasibilityEvaluated implicit pointer conversion feasibilityYes. char * implicitly converts to void * in C, so the cast is entirely unnecessary:
creturn vector->data + (idx * vector->data_size);
The arithmetic is well-defined on char *, and the implicit conversion to void * on return is clean. The cast was only needed if you kept the (void *) before the arithmetic, which was the source of the UB.#ifndef VECTOR_H
#define VECTOR_H

#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
        char *data;
        size_t data_size;
        size_t count;
        size_t capacity;
} vector_t;

vector_t *vector_new(size_t data_size);
void vector_free(vecpasted11:22 AMThis looks correct to me. No bugs to report.
A few small observations if you want to polish further:

The macros being defined and undef'd inside the function body works, but if you want them user-configurable (as discussed earlier), move them to the top of the header before the declarations so users can override them before including:

c#ifndef VECTOR_INITIAL_CAPACITY
#define VECTOR_INITIAL_CAPACITY 4
#endif

#ifndef VECTOR_GROWTH_FACTOR
#define VECTOR_GROWTH_FACTOR 2
#endif

vector_is_empty returns int but stdbool.h is already included — bool would be more expressive, and consistent with how most users will treat the return value.
vector_remove silently does nothing on an out-of-bounds idx by relying on vector_at returning NULL. That's fine, but worth a comment since the behavior isn't obvious from the signature.

Otherwise this is clean, well-structured, and ready to use as a base for the umap.Do I also use Doxygen comments here?11:22 AMYes, it's a natural fit for a header-only library. The header is the entire public interface, so documentation lives there anyway. A lightweight approach that doesn't clutter the code:
c/**
 * @brief Creates a new vector.
 * @param data_size Size in bytes of each element.
 * @return Pointer to the new vector, or NULL on failure.
 */
vector_t *vector_new(size_t data_size);

/**
 * @brief Inserts an element at the given index.
 * @param vector The vector.
 * @param idx Index to insert at. Must be <= count.
 * @param data_in Pointer to the element to copy in.
 * @return Pointer to the inserted element, or NULL on failure.
 * @warning The returned pointer is invalidated by any subsequent
 *          operation that triggers a reallocation.
 */
void *vector_insert(vector_t *vector, size_t idx, const void *data_in);
Practical advice:

Put Doxygen comments on the declarations in the header only, not the definitions — no point duplicating them
The @warning tag is especially useful here given the pointer invalidation risk discussed earlier
You don't need to document trivial ones like vector_front and vector_back heavily — a single @brief is enough
Add a file-level block at the top:

c/**
 * @file vector.h
 * @brief Generic dynamic array for C.
 *
 * Define VECTOR_IMPLEMENT in exactly one translation unit before
 * including this header to instantiate the definitions.
 */
It's low overhead for a library others might use, and it forces you to think about preconditions and failure cases as you write it.I accidental messed up my doxygen commenst but have docs, can I retrieve them?11:28 AMUnfortunately no — if you've overwritten or deleted the comments in your source file, I can't retrieve them. A few options depending on your situation:
If you're using Git:
bashgit log vector.h          # find the commit before the accident
git show <commit>:vector.h  # view that version
git checkout <commit> -- vector.h  # restore it
If you're on macOS/Linux:

Check if your editor has a local history feature (VS Code does under "Timeline" in the file explorer)
Look for editor swap files (.vector.h.swp for Vim)

If you have the generated Doxygen HTML/XML output:

The docs themselves contain all the text — you can manually reconstruct the comments from them
If you have XML output (GENERATE_XML = YES in Doxyfile), it's especially easy to parse back

If none of the above:

Share the generated HTML docs or any snapshot and I can reconstruct the comment block for you
Or share the current header and I can write the Doxygen comments fresh based on what we've discussed in this conversation

Which situation are you in?How do I share the doxygen docs?11:29 AMA few ways:
If you have the generated HTML folder:

Zip the entire html/ output folder and upload it here
Or just upload the main index.html — though without the supporting files it may not render fully

If you have XML output:

Upload the individual .xml files from the xml/ folder — these are the most useful for reconstructing comments since they're structured data

Easiest option:

Just upload the relevant .html file for vector.h (usually named something like vector_8h.html) — I can read the raw HTML and extract all the documentation text from it

Which output format did you generate?html.zipzip11:29 AMRan 3 commands, viewed a fileRan 3 commands, viewed a fileThis isn't your vector library's docs — it's Doxygen output for a different project involving a lexer, parser, and AST (lexer.h, parser.h, common.h, main.c). There's no vector.h anywhere in the zip.
You likely generated these docs from a different project directory. To get the vector docs you'll need to either:

Run Doxygen again pointed at the correct directory containing vector.h
Or just share the current vector.h and I'll write the Doxygen comments fresh — since we've reviewed every function in detail throughout this conversation, I have all the context needed to do it accurately
Yes, these are from my project and I messed up comments from that project.11:30 AMGot it, my mistake. Let me extract the relevant files.Ran 3 commandsRan 3 commandsGot everything. Here are the reconstructed Doxygen comments for all three headers:

common.h
c/**
 * @file common.h
 * @brief Shared utilities.
 */

/** @brief The file extension for Chalk source files. */
#define FILE_EXTENSION ".chalk"

/** @brief Length of FILE_EXTENSION, excluding the null terminator. */
#define FILE_EXTENSION_LEN (sizeof(FILE_EXTENSION) - 1)

/**
 * @brief Immediately terminate the process with an error message.
 *
 * Prints @p msg to stderr prefixed with "Error: " and calls exit(EXIT_FAILURE).
 * Marked noreturn - the compiler knows control never returns from this call.
 *
 * @param msg Message to print before exiting.
 */
void die(const char *msg) __attribute__((noreturn));

/**
 * @brief Allocate memory, dying on failure.
 *
 * Thin wrapper around malloc(). If allocation fails, calls die() rather than
 * returning NULL - callers never need to check the return value.
 *
 * @param size Number of bytes to allocate.
 * @return Pointer to allocated memory. Never NULL.
 */
void *mem_alloc(size_t size);

/**
 * @brief Free allocated memory.
 *
 * Wrapper around free(). Passing NULL is safe.
 *
 * @param ptr Pointer to free.
 */
void mem_free(void *ptr);

lexer.h
c/**
 * @file lexer.h
 * @brief Lexer API: tokenisation of Chalk source text.
 */

/** @brief Maximum number of characters in an identifier. */
#define MAX_IDENT_LEN 32

/**
 * @brief All token types produced by the lexer.
 *
 * TOK_UNKNOWN is returned for any lexeme the lexer does not recognise.
 * TOK_EOF signals end of input.
 */
typedef enum token_type_t {
    TOK_NUM,      /**< Numeric literal (integer or float). */
    TOK_ADD,      /**< '+' operator. */
    TOK_SUB,      /**< '-' operator. */
    TOK_MUL,      /**< '*' operator. */
    TOK_DIV,      /**< '/' operator. */
    TOK_POW,      /**< '^' operator. */
    TOK_LPAREN,   /**< '(' operator. */
    TOK_RPAREN,   /**< ')' operator. */
    TOK_PRINT,    /**< 'print' keyword. */
    TOK_COLON,    /**< ':' separator. */
    TOK_SCOLON,   /**< ';' statement terminator. */
    TOK_LET,      /**< 'let' keyword. */
    TOK_IDENT,    /**< Identifier. */
    TOK_EOF,      /**< End of source input. */
    TOK_UNKNOWN   /**< Unrecognised character. */
} token_type_t;

/**
 * @brief Opaque lexer handle.
 *
 * Internal layout is defined in lexer.c. Callers interact only via the
 * lexer_*() API - never access the fields directly.
 */
typedef struct _lexer lexer_t;

/**
 * @brief Create a new lexer for the given source string.
 *
 * The lexer does not copy @p src - the caller must keep it alive for
 * the lexer's lifetime.
 *
 * @param src Null-terminated source program. Must not be NULL.
 * @return Newly allocated lexer. Free with lexer_free().
 */
lexer_t *lexer_new(const char *src);

/**
 * @brief Free a lexer and set its pointer to NULL.
 *
 * Safe to call with a NULL @p lexer_addr or a NULL @p *lexer_addr.
 *
 * @param lexer_addr Address of the lexer pointer to free and nullify.
 */
void lexer_free(lexer_t **lexer_addr);

/**
 * @brief Consume and return the next token.
 *
 * If a peeked token is cached, returns and clears it without re-scanning.
 *
 * @param lexer Active lexer. Must not be NULL.
 * @return Next token in the stream.
 */
token_t lexer_next(lexer_t *lexer);

/**
 * @brief Return the next token without consuming it.
 *
 * Subsequent calls to lexer_peek() return the same token until
 * lexer_next() is called.
 *
 * @param lexer Active lexer. Must not be NULL.
 * @return Next token in the stream (same as the next lexer_next() call).
 */
token_t lexer_peek(lexer_t *lexer);

/**
 * @brief Dump the full token stream to @p stream.
 *
 * Consumes all remaining tokens. Calls die() on TOK_UNKNOWN.
 *
 * @param lexer Active lexer. Must not be NULL.
 * @param stream Output stream. Must not be NULL.
 */
void lexer_dump(lexer_t *lexer, FILE *stream);

parser.h
c/**
 * @file parser.h
 * @brief Parser API: recursive descent parser, AST definition, evaluator.
 */

/**
 * @brief AST node types.
 *
 * Controls how ast_eval() and ast_dump() interpret a node and its children.
 */
typedef enum {
	AST_NUM,     /**< Numeric literal leaf. */
	AST_NEG,     /**< Unary negation. */
	AST_ADD,     /**< Binary addition. */
	AST_SUB,     /**< Binary subtraction. */
	AST_MUL,     /**< Binary multiplication. */
	AST_DIV,     /**< Binary division. */
	AST_POW,     /**< Exponentiation. */
	AST_PRINT,   /**< Print statement. */
	AST_LET,     /**< Let statement. */
	AST_IDENT,   /**< Identifier. */
	AST_PROGRAM  /**< Program node - chains statements. */
} ast_node_type_t;

/**
 * @brief Opaque AST node.
 *
 * Internal layout defined in parser.c. All access is through
 * parse_program(), ast_eval(), and ast_dump().
 */
typedef	struct _ast_node ast_node_t;

/**
 * @brief Parse a complete Chalk program into an AST.
 *
 * Repeatedly calls _parse_statement() until TOK_EOF. Each call returns
 * an AST_PROGRAM node whose left child is the parsed statement and whose
 * right child is the rest of the program (or NULL at end of input).
 *
 * @param lexer Active lexer positioned at the start of input. Must not be NULL.
 * @return Root AST_PROGRAM node, or NULL if the input is empty.
 */
void *parse_program(lexer_t *lexer);

/**
 * @brief Evaluate an AST and write output to @p stream.
 *
 * Walks the tree recursively and acts based on the active node.
 *
 * @param node Root node to evaluate.
 * @param stream Output stream for print statements.
 */
void ast_eval(ast_node_t *node, FILE *stream);


/**
 * @brief Dump a human-readable representation of the AST to @p stream.
 *
 * Indentation reflects tree depth (1 space per level).
 *
 * @param node Root node. Must not be NULL.
 * @param stream Output stream. Must not be NULL.
 */
void ast_dump(ast_node_t *node, FILE *stream);

#endif

