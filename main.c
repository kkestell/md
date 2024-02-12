#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "utf8.h"

FILE *ifp = NULL, *ofp = NULL;
utf8_int32_t current = 0, next = 0;

void parse_text(bool multiline);
void parse_heading(void);
void parse_unordered_list(void);
void parse_ordered_list(void);
void parse_code_block(void);
void parse_blockquote(void);
void parse_horizontal_rule(void);
void parse_paragraph(void);
void parse_html_block(void);
void parse_table(void);
void advance(void);
void pop(int n);
void skip_whitespace(void);
void skip_line(void);
void put_codepoint(utf8_int32_t codepoint);
void put_string(const char *str);

int main(int argc, char *argv[])
{
    int opt;
    char *input_filename = NULL;
    char *output_filename = NULL;

    // Parse command-line arguments
    while ((opt = getopt(argc, argv, "i:o:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            input_filename = optarg;
            break;
        case 'o':
            output_filename = optarg;
            break;
        default:
            fprintf(stderr, "Usage: %s [-i input_file] [-o output_file]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Open input file or stdin
    if (input_filename)
    {
        ifp = fopen(input_filename, "r");
        if (!ifp)
        {
            perror("Error opening input file");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        ifp = stdin;
    }

    // Open output file or stdout
    if (output_filename)
    {
        ofp = fopen(output_filename, "w");
        if (!ofp)
        {
            perror("Error opening output file");
            if (ifp != stdin)
                fclose(ifp);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        ofp = stdout;
    }

    advance();
    advance();

    while (true)
    {
        utf8_int32_t codepoint = current;

        if (codepoint == 0)
            break;

        switch (codepoint)
        {
        case '<':
            parse_html_block();
            break;
        case '#':
            parse_heading();
            break;
        case '*':
            parse_unordered_list();
            break;
        case '1':
            parse_ordered_list();
            break;
        case '`':
            parse_code_block();
            break;
        case '>':
            parse_blockquote();
            break;
        case '-':
            parse_horizontal_rule();
            break;
        case '|':
            parse_table();
            break;
        default:
            parse_paragraph();
            break;
        }

        while (current == '\n')
            pop(1);
    }

    if (ifp != stdin)
        fclose(ifp);
    if (ofp != stdout)
        fclose(ofp);

    return 0;
}

void parse_text(bool multiline)
{
    bool is_bold = false;
    bool is_italic = false;
    bool is_code = false;

    while (true)
    {
        if (current == 0)
            break;

        if (current == '\n')
        {
            if (next == '\n' || !multiline)
                break;

            put_string("<br>");
            pop(1);
            continue;
        }

        if (current == '!' && next == '[')
        {
            pop(2);
            put_string("<a href=\"");
            while (current != ']')
            {
                put_codepoint(current);
                pop(1);
            }
            pop(1); // ]
            pop(1); // (
            put_string("\">");
            while (current != ')')
            {
                put_codepoint(current);
                pop(1);
            }
            pop(1); // )
            put_string("</a>");
            continue;
        }

        if (current == '_')
        {
            is_italic = !is_italic;
            put_string(is_italic ? "<i>" : "</i>");
            pop(1);
        }

        if (current == '*')
        {
            is_bold = !is_bold;
            put_string(is_bold ? "<b>" : "</b>");
            pop(1);
        }

        if (current == '`')
        {
            is_code = !is_code;
            put_string(is_code ? "<code>" : "</code>");
            pop(1);
        }

        put_codepoint(current);
        pop(1);
    }
}

void parse_heading(void)
{
    int level = 0;
    while (current == '#')
    {
        level++;
        pop(1); // #
    }
    put_string("<h");
    put_codepoint('0' + level);
    put_string(">");
    skip_whitespace();
    parse_text(false);
    put_string("</h");
    put_codepoint('0' + level);
    put_string(">\n");
}

void parse_unordered_list(void)
{
    put_string("<ul>\n");
    while (current == '*')
    {
        pop(1); // *
        skip_whitespace();
        put_string("<li>");
        parse_text(false);
        put_string("</li>\n");
        pop(1); // \n
    }
    put_string("</ul>\n");
}

void parse_ordered_list(void)
{
    put_string("<ol>\n");
    int index = 1;
    while (current == '0' + index)
    {
        pop(2); // 1.
        skip_whitespace();
        put_string("<li>");
        parse_text(false);
        put_string("</li>\n");
        pop(1); // \n
        index++;
    }
    put_string("</ol>\n");
}

void parse_code_block(void)
{
    pop(4); // ```\n
    put_string("<pre><code>");
    while (current != '`' && current != 0)
    {
        put_codepoint(current);
        pop(1);
    }
    put_string("</code></pre>\n");
    pop(3); // ```
}

void parse_blockquote(void)
{
    put_string("<blockquote>");
    while (current == '>')
    {
        pop(1); // >
        skip_whitespace();
        parse_text(false);
        pop(1); // \n
        if (current == '>')
            put_string("<br>");
    }
    put_string("</blockquote>\n");
}

void parse_horizontal_rule(void)
{
    pop(3); // ---
    put_string("<hr>\n");
}

void parse_table(void)
{
    put_string("<table>\n");

    while (true)
    {
        pop(1); // |

        if (current == '-')
        {
            skip_line();
            continue;
        }

        put_string("<tr>");

        while (true)
        {
            skip_whitespace();

            put_string("<td>");

            while (current != '|')
            {
                if (current == ' ' && (next == ' ' || next == '|'))
                {
                    pop(1);
                    continue;
                }

                put_codepoint(current);
                pop(1);
            }

            pop(1); // |

            put_string("</td>");

            if (current == '\n' || current == 0)
                break;
        }

        pop(1); // \n

        put_string("</tr>\n");

        if (current == '\n')
            break;
    }

    put_string("</table>\n");
}

void parse_paragraph(void)
{
    put_string("<p>");
    parse_text(true);
    put_string("</p>\n");
}

void parse_html_block(void)
{
    put_string("<");
    pop(1); // <

    while (!(current == '\n' && next == '\n') && current != 0)
    {
        if (current == '<')
        {
            put_string("<");
            pop(1);
        }
        else
        {
            put_codepoint(current);
            pop(1);
        }
    }

    if (current == '\n' && next == '\n')
    {
        pop(2); // \n\n
    }

    put_string("\n");
}

utf8_int32_t utf8getc(void)
{
    unsigned char first_byte, additional_bytes[3];
    int num_additional_bytes = 0;
    utf8_int32_t codepoint = 0;

    // Read the first byte
    if (fread(&first_byte, 1, 1, ifp) != 1)
    {
        next = 0; // End of file or error
        return 0;
    }

    // Determine the number of additional bytes needed based on the first byte
    // 0xxxxxxx
    if ((first_byte & 0x80) == 0x00)
    {
        num_additional_bytes = 0;
        codepoint = first_byte;
    }
    // 110xxxxx
    else if ((first_byte & 0xE0) == 0xC0)
    {
        num_additional_bytes = 1;
    }
    // 1110xxxx
    else if ((first_byte & 0xF0) == 0xE0)
    {
        num_additional_bytes = 2;
    }
    // 11110xxx
    else if ((first_byte & 0xF8) == 0xF0)
    {
        num_additional_bytes = 3;
    }
    // Invalid UTF-8 start byte
    else
    {
        return 0;
    }

    if (num_additional_bytes > 0)
    {
        if (fread(additional_bytes, 1, num_additional_bytes, ifp) != num_additional_bytes)
        {
            return 0;
        }

        switch (num_additional_bytes)
        {
        case 1:
            codepoint = ((first_byte & 0x1F) << 6) | (additional_bytes[0] & 0x3F);
            break;
        case 2:
            codepoint =
                ((first_byte & 0x0F) << 12) | ((additional_bytes[0] & 0x3F) << 6) | (additional_bytes[1] & 0x3F);
            break;
        case 3:
            codepoint = ((first_byte & 0x07) << 18) | ((additional_bytes[0] & 0x3F) << 12) | ((additional_bytes[1] & 0x3F) << 6) | (additional_bytes[2] & 0x3F);
            break;
        }
    }

    return codepoint;
}

void advance(void)
{
    current = next;
    next = utf8getc();
}

void pop(int n)
{
    for (int i = 0; i < n; i++)
        advance();
}

void skip_whitespace(void)
{
    while (current == ' ')
    {
        advance();
    }
}

void skip_line(void)
{
    while (current != '\n' && current != 0)
    {
        advance();
    }

    if (current == '\n')
    {
        advance();
    }
}

void put_codepoint(utf8_int32_t codepoint)
{
    utf8_int8_t encoded[5] = {0};
    utf8catcodepoint(encoded, codepoint, sizeof(encoded));
    fputs((char *)encoded, ofp);
}

void put_string(const char *str)
{
    fputs(str, ofp);
}
