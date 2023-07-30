def vizinho(old_S):
    S = Cluster(old_S.makespan, old_S.machines)
    val = S.makespan
    index = 0
    for i in range(0, len(S.machines)):
        new = 0
        for tk in S.machines[i]:
            new += tk
        if val == new:
            index = i
            break

    task = S.machines[index][-1:][0]
    S.machines[index].pop()

    found = False
    for i in range(0, len(S.machines)):
        new = 0
        for tk in S.machines[i]:
            new = new + tk
        new = new + task
        if new < val:
            found = True
            S.machines[i].append(task)
            break

    if found == False:
        S.machines[index].append(task)
    else:
        new_make = 0
        for mach in S.machines:
            new = 0
            for tk in mach:
                new += tk
            if new_make < new:
                new_make = new
        S.makespan = new_make
    return S
