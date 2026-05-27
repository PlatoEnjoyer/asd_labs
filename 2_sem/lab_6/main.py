def rabin_karp_search(pattern, text, prime=101):
    if not pattern or not text or len(pattern) > len(text):
        return []
    
    m, n = len(pattern), len(text)
    base = 256
    
    # base^(m-1) по модулю prime
    h = pow(base, m-1, prime)
    
    # Вычисляем хэши
    pattern_hash = 0
    text_hash = 0
    
    for i in range(m):
        pattern_hash = (base * pattern_hash + ord(pattern[i])) % prime
        text_hash = (base * text_hash + ord(text[i])) % prime
    
    results = []
    
    for i in range(n - m + 1):
        if pattern_hash == text_hash:
            if text[i:i+m] == pattern:
                results.append(i)
        
        if i < n - m:
            text_hash = (base * (text_hash - ord(text[i]) * h) + ord(text[i + m])) % prime
            if text_hash < 0:
                text_hash += prime
    
    return results



print(rabin_karp_search("abc", "abcabcabc"))
print(rabin_karp_search("test", "this is a test string"))
print(rabin_karp_search("xyz", "abcdefghijk"))