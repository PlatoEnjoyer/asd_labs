#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void kmp_search(const char *text, const char *pattern) {
    int n = strlen(text);
    int m = strlen(pattern);
    
    if (m == 0 || n == 0) return;
    
    int lps[m];
    lps[0] = 0;
    
    for (int i = 1, len = 0; i < m; ) {
        if (pattern[i] == pattern[len]) {
            lps[i++] = ++len;
        } else if (len) {
            len = lps[len - 1];
        } else {
            lps[i++] = 0;
        }
    }
    
    printf("Вхождения '%s' в '%s':\n", pattern, text);
    
    int found = 0;
    for (int i = 0, j = 0; i < n; ) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
        }
        
        if (j == m) {
            printf("  На позиции %d\n", i - j);
            found = 1;
            j = lps[j - 1];
        } else if (i < n && text[i] != pattern[j]) {
            j ? (j = lps[j - 1]) : i++;
        }
    }
    
    if (!found) {
        printf("  Не найдено\n");
    }
}

int main() {
    kmp_search("ababcabcabababd", "ababd");
    return 0;
}