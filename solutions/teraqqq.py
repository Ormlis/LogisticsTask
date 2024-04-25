import random

def solve():
    k, n = map(int, input().split())
    
    g = [[] for i in range(k)]
    mp = [-1] * n
    for v in range(n):
        gv = list(map(lambda s: int(s)-1, input().split()))[1:]
        for u in gv: g[u].append(v)
    work = []

    f, s = 0, -1
    used, utimer = [], 0
    while f != n:
        def Kuhn(v, used):
            if used[v] == utimer: return False
            used[v] = utimer
            for to in g[v%k]:
                if mp[to] == -1 or Kuhn(mp[to], used):
                    mp[to] = v
                    return True
            return False
        if s % n == 0:
            for w in g: random.shuffle(w)
        s += 1
        utimer += 1
        used.append(0)
        if Kuhn(s, used):
            f += 1
    
    work = [[] for _ in range(s//k+1)]
    for i in range(n):
        work[mp[i]//k].append([i, mp[i]%k])
    
    print(len(work))
    for day in work:
        print(len(day))
        for a, b in day:
            print(a+1, b+1)


t = int(input())
for _ in range(t):
    solve()