# Resourceful Students — Combinatorial Optimization Project

## Overview
This project addresses the **“Resourceful Students”** problem, a combinatorial optimization challenge from the Optil.io platform.  
The problem is formulated as a **graph optimization task**, where each student is represented by a node, and each friendship between two students is represented by an edge.

The objective is to select a **minimal set of pairs (edges)** such that:
- Every student belongs to at most one selected pair.
- Among all unpaired students, no two are friends (there is no edge connecting them).

Two different algorithmic strategies were implemented:
- **RSE.cpp** — an exact backtracking algorithm
- **RSH.cpp** — a heuristic method combining simulated annealing and greedy selection

Both versions were tested and submitted to Optil.io and achieved competitive results.

---

Developed by:
- Cezary Suchorski
- Michał Żarnowski
- Dariia Shevchuk
- Piotr Szymiec
