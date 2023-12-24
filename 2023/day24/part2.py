from sympy import Eq, symbols, solve

def parse_vec3(l):
    return list(map(int, l.split(", ")))

def parse_line(l):
    pos_vel = l.split(" @ ")
    return parse_vec3(pos_vel[0]) + parse_vec3(pos_vel[1])

lines = []
with open("2023/day24/input.txt", "r") as f:
    lines = f.readlines()
    lines = list(map(lambda l: parse_line(l), lines))

# solution line equation
x0, y0, z0 = symbols("x0 y0 z0", integer=True)
vx0, vy0, vz0 = symbols("vx0 vy0 vz0", integer=True)

eqs = []

for i, line in enumerate(lines[:3]):
    t = symbols(f" t{i} ")
    x, y, z, vx, vy, vz = line
    eqs.append(Eq(x0 + vx0*t, x + vx*t))
    eqs.append(Eq(y0 + vy0*t, y + vy*t))
    eqs.append(Eq(z0 + vz0*t, z + vz*t))

solution = solve(eqs)[0]
print(solution[x0],solution[y0],solution[z0], "@", solution[vx0], solution[vy0], solution[vz0])

answer = sum([solution[v] for v in [x0, y0, z0]])
print(answer)
