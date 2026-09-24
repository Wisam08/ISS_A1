import pandas as pd
import matplotlib.pyplot as plt


# ============================================================
# QUESTION 1
# ============================================================

q1 = pd.read_csv("question1_results.csv")

plt.figure(figsize=(8, 5))

for case in q1["case"].unique():

    data = q1[q1["case"] == case]

    plt.plot(
        data["n"],
        data["time_ms"],
        marker="o",
        label=case
    )

plt.xlabel("Input size (n)")
plt.ylabel("Time (ms)")
plt.title("Question 1: Runtime vs Input Size")
plt.xscale("log")
plt.yscale("log")
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.savefig("question1_runtime.png", dpi=200)
plt.show()


# ============================================================
# QUESTION 2
# ============================================================

q2 = pd.read_csv("question2_results.csv")

plt.figure(figsize=(8, 5))

for case in q2["case"].unique():

    data = q2[q2["case"] == case]

    plt.plot(
        data["n"],
        data["time_ms"],
        marker="o",
        label=case
    )

plt.xlabel("Number of nodes (n)")
plt.ylabel("Time (ms)")
plt.title("Question 2: Runtime vs Tree Size")
plt.xscale("log")
plt.yscale("log")
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.savefig("question2_runtime.png", dpi=200)
plt.show()


# ============================================================
# QUESTION 3 — VARY N
# ============================================================

q3 = pd.read_csv("question3_results.csv")

q3_N = q3[q3["C"] == 6]

plt.figure(figsize=(8, 5))

plt.plot(
    q3_N["N"],
    q3_N["time_ms"],
    marker="o"
)

plt.xlabel("Number of graph nodes (N)")
plt.ylabel("Time (ms)")
plt.title("Question 3: Runtime vs Graph Size")
plt.xscale("log")
plt.yscale("log")
plt.grid(True)

plt.tight_layout()
plt.savefig("question3_graph_scaling.png", dpi=200)
plt.show()


# ============================================================
# QUESTION 3 — VARY C
# ============================================================

q3_C = q3[q3["N"] == 1000]

plt.figure(figsize=(8, 5))

plt.plot(
    q3_C["C"],
    q3_C["time_ms"],
    marker="o"
)

plt.xlabel("Number of packages (C)")
plt.ylabel("Time (ms)")
plt.title("Question 3: Runtime vs Number of Packages")
plt.grid(True)

plt.tight_layout()
plt.savefig("question3_package_scaling.png", dpi=200)
plt.show()


# ---------------------------------------------
# import pandas as pd
# import matplotlib.pyplot as plt


# Load results
data = pd.read_csv("question3_vs_N.csv")


# Plot
plt.figure(figsize=(8, 5))

plt.plot(
    data["N"],
    data["time_ms"],
    marker="o"
)

plt.xlabel("Number of graph nodes (N)")
plt.ylabel("Runtime (ms)")
plt.title("Question 3: Runtime vs Graph Size")


# Log scale is useful because the input sizes
# span a large range.
plt.xscale("log")
plt.yscale("log")

plt.grid(True)

plt.tight_layout()

plt.savefig(
    "question3_runtime_vs_N.png",
    dpi=200
)

plt.show()

# import pandas as pd
# import matplotlib.pyplot as plt


# Load results
data = pd.read_csv("question3_vs_C.csv")


# Plot
plt.figure(figsize=(8, 5))

plt.plot(
    data["C"],
    data["time_ms"],
    marker="o"
)

plt.xlabel("Number of packages (C)")
plt.ylabel("Runtime (ms)")
plt.title("Question 3: Runtime vs Number of Packages")

plt.grid(True)

plt.tight_layout()

plt.savefig(
    "question3_runtime_vs_C.png",
    dpi=200
)

plt.show()