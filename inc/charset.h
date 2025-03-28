#ifndef CUSTOM_CHARSET_H
#define CUSTOM_CHARSET_H

#include <stdint.h>
#include <stddef.h> // for size_t

// Define type for characters in your charset
typedef uint8_t custom_char_t;

// Enumeration of all characters in the custom charset
typedef enum {
    CHAR_space = 0,
    CHAR_a , // 1
    CHAR_b , // 2
    CHAR_c , // 3
    CHAR_d , // 4
    CHAR_e , // 5
    CHAR_f , // 6
    CHAR_g , // 7
    CHAR_h , // 8
    CHAR_i , // 9
    CHAR_j , // 10
    CHAR_k , // 11
    CHAR_l , // 12
    CHAR_m , // 13
    CHAR_n , // 14
    CHAR_o , // 15
    CHAR_p , // 16
    CHAR_q , // 17
    CHAR_r , // 18
    CHAR_s , // 19
    CHAR_t , // 20
    CHAR_u , // 21
    CHAR_v , // 22
    CHAR_w , // 23
    CHAR_x , // 24
    CHAR_y , // 25
    CHAR_z , // 26
    CHAR_0,  // 27
    CHAR_1,  // 28
    CHAR_2,  // 29
    CHAR_3,  // 30
    CHAR_4,  // 31
    CHAR_5,  // 32
    CHAR_6,  // 33
    CHAR_7,  // 34
    CHAR_8,  // 35
    CHAR_9,  // 36
    CHAR_ü,  // 37
    CHAR_question,   // ?
    CHAR_exclaim,    // !
    CHAR_colon,      // :
    CHAR_equal,      // =
    CHAR_grave,      // `
    CHAR_dot,        // .
    CHAR_comma,      // ,
    CHAR_slash,      // /
    CHAR_backslash,  // \
    CHAR_plus,       // + 47
    CHAR_minus,      // -
    CHAR_quote,      // "
    CHAR_apostrophe, // '
    CHAR_lparen,     // (
    CHAR_rparen,     // )
    CHAR_symbola,    // \symbola - christ
    CHAR_hash,       // #
    CHAR_tilde,      // ~
    CHAR_symbolb,    // \symbolb
    CHAR_symbolc,    // \symbolc
    CHAR_symbold,    // \symbold
    CHAR_symbole,    // \symbole
    CHAR_asterisk,   // *
    CHAR_symbolf,    // \symbolf
    CHAR_symbolg,    // \symbolg
    CHAR_underscore, // _
    CHAR_COUNT       // Total count of characters
} custom_charset_t;

// // Reverse mapping for debugging
// static const char *custom_charset_utf8[CHAR_COUNT] = {
//     " ","a", "b", "c", "d", "e", "f", "g", "h", "i", "j",
//     "k", "l", "m", "n", "o", "p", "q", "r", "s", "t",
//     "u", "v", "w", "x", "y", "z",
//     "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
//     "ü", "?", "!", ":", "=", "`", ".", ",", "/",
//     "\\", "+", "-", "\"", "'", "(", ")", "\\symbola", "#", "~",
//     "\\symbolb", "\\symbolc", "\\symbold", "\\symbole", "*", "\\symbolf", "\\symbolg", "_"
// };

// // --- Function to convert plain text to custom charset ---
// size_t utf8_to_custom_charset(const char *input, custom_char_t *output, size_t max_output_len);

#endif // CUSTOM_CHARSET_H
