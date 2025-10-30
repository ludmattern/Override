login = "jgavairo"

val = ((ord(login[3]) ^ 0x1337) + 0x5eeded)
for ch in login:
    val = (val + ((ord(ch) ^ val) % 0x539))

print(val)
