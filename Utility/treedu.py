import os

def sizeof_fmt(num, hr=True, suffix=""):
    if not hr:
        return "%i"%num

    for unit in (" ", "K", "M", "G", "T", "P", "E", "Z"):
        if abs(num) < 1024.0:
            return f"{num:3.1f}{unit}{suffix}"
        num /= 1024.0
    return f"{num:.1f}Yi{suffix}"

def recursivePrint(rd, hr, depth = 0, mask = 0):
    for i, rdname in enumerate(rd):
        if depth:
            padding = "".join(["│         " if ((1 << (i)) & mask) else "          " for i in range(depth-1)])
            print("".join(["  ", padding, "├" if (i != (len(rd)-1)) else "└", "────", "%s"%sizeof_fmt(rd[rdname][0], hr).rjust(7 if hr else 12, " "), "  ", rdname]))
        else:
            print("%s"%sizeof_fmt(rd[rdname][0], hr), rdname)
        if len(rd[rdname][1]) > 0:
            recursivePrint(rd[rdname][1], hr, depth + 1, (1 << depth) | (mask if (i != (len(rd)-1)) else (mask & ~(1 << ((depth-1) if depth > 1 else 0)))))

def recursiveSort(rd):
    for name, item in rd.items():
        if len(item[1]) > 0:
            item[1] = recursiveSort(item[1])
    return dict(sorted(rd.items(), key=lambda item: -item[1][0]))

if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(
                    prog='dutree',
                    description='Recursively display directory struture with directory size')

    parser.add_argument('path', help="Root path to list from")           # positional argument
    parser.add_argument('-d', '--depth', default=3, type=int, help="Recursion depth")      # option that takes a value
    parser.add_argument('-b', '--bytes', help="Display size in bytes instead of human readable format",
                        action='store_true')  # on/off flag
    parser.add_argument('-s', '--sort', help="Sorts directories by size",
                        action='store_true')  # on/off fla

    args = parser.parse_args()

    # use du to efficiently acquire data 
    fsr = os.popen("du -b %s -d %i"%(args.path, args.depth))
    data = fsr.read()
    fsr.close()

    # parse du results 
    resultDict = {}
    for line in data.split("\n"):
        if "Permission Denied" in line:
            continue

        try:
            size, path = line.split("\t")
        except(ValueError):
            continue

        size = int(size)

        trd = [0, resultDict]
        for name in path.strip("/").split("/"):
            if not name in trd[1]:
                trd[1][name] = [0, {}]
            trd = trd[1][name]
        trd[0] = size

    if args.sort:
        resultDict = recursiveSort(resultDict)

    # display results 
    recursivePrint(resultDict, not args.bytes)
