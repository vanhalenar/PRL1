import sys

def main():
    numbers = list(map(int, sys.stdin.read().split()))
    print(" ".join(map(str, numbers)))
    numbers.sort()
    for num in numbers:
        print(num)
   

if __name__ == "__main__":
    main()