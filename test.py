import subprocess
import filecmp

def prep_test(proc):
    with open("oets.out", "w") as outfile:
        subprocess.run(["./run.sh", str(proc)], stdout=outfile)

    with open("oets.out", "r") as infile:
        nums = infile.readline().strip()

    with open("ref.out", "w") as outfile:
        subprocess.run(["python3", "sort.py"], input=nums.encode(), stdout=outfile)

def run_tests(proc, repeats):
    for i in range(1, repeats+1):
        difference=False
        prep_test(proc)
        with open("oets.out") as f1, open("ref.out") as f2:
            for line1, line2 in zip(f1, f2):
                if line1.rstrip() != line2.rstrip():
                    print("Difference found:")
                    difference=True
                    print(f"File1: {line1.strip()}")
                    print(f"File2: {line2.strip()}")
        if (difference == False):
            print(f"{proc}: Passed test {i} of {repeats}")


run_tests(8, 5)
run_tests(12, 5)
run_tests(17, 10)
run_tests(16, 6)
run_tests(19, 10)

subprocess.run("rm *.out", shell=True)