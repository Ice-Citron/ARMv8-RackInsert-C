# RackInsert-C

RackInsert-C is a standalone robotics C/MuJoCo benchmark for a staged robotic
cable inseertion task. It loads a MuJoCO scene (from Google's AIC 
competition), runs a simple staged insertion policy (hardcoded for now), 
evaluates plug-to-socket geometry, scores each trial, and can record 
trajectories for replay.

## Motivation
This project was build as an Imperial C Project Extension to explore robotics
simulation, MuJoCo, finite-state control, and benchmark/evaluator design in C.

The scene assets are sourced from [Intrinsic's AI for Industry Challenge](https://www.intrinsic.ai/events/ai-for-industry-challenge).
The scoring designed is also inspired by the AIC evalouator structure: model
validity, motion quality, and insertion success. The evaluator we built here is
a simplified independent C implementation by us, not the official AIC scorer.

## Features
- Loads MuJoCo MJCF scenes from MuJoCo's C API.
- Uses AIC-derived robot, task-board, cable and TCP assets.
- During demo, runs a staged scripted policy:
    - APPROACH (server rack)
    - HOVER    (hover above cable socket)
    - ALIGN    (aligns plug about socket before inertion)
    - INSERT
- During evaluation, computes plug-tip to socket geometry:
    - plug-port distance
    - lateral error of TCP to socket
    - axial insertion depth of TCP
- Scores trials using 3-tier AIC-inspired score rubric
- Writes CSV traces for post-demo replay (can be used for training Robotic 
  Models, e.g. Action Chunking Transformers)
- Includes unit/smoke tests for evaluators, actuators, and scripted policy.

## Repository Layout

```
extension/
    Makefile
    assets/mujoco/rack_insert/     # AIC-derived MJCF and mesh assets
    include/                       # public C headers
    src/                           # C implementation
    tests/                         # C tests and smoke tests
    scripts/replay_trace.py        # Python MuJoCo trace viewer
```

## Build

```bash
cd armv8_64/extension
make deps
make clean all
```

`make deps` creates a local Python virtual environment and installs the 
Python `mujoco` library. Our Makefile then auto-detects the MuJoCo headers from
this environment. `make clean` deletes all compiled binaries and log files. 
`make all` compiles `./rack_insert` which is required for demo.

## Run Benchmark

```bash
./rack_insert
```

Run multiple trials:

```bash
./rack_insert --trials 10
```

Use a custom scene:

```bash
./rack_insert --scene assets/mujoco/rack_insert/rack_insert_scene_cable_softplugin_rollout.xml --trials 3
```

# Record and Replay

Record a rollout:

```bash
./rack_insert --trace results.csv
```

Replay it in the MuJoCo viewer:

```bash
.venv/bin/python scripts/replay_trace.py --trace results.csv --speed 1.0
```

Or use:

```bash
make demo
```

## Tests

```bash
make test
```

This runs:
- `test_evaluator`: checks evaluator geometry and scoring logic.
- `test_actuators`: checks MuJoCo actuator integration with program.
- `test_scripted_policy`: checks that the staged policy runs properly (smoke 
  test).

## Architecture

- `sim.c` wraps MuJoCo loading, plugin loading, reset, stepping, site lookup, 
  joint lookup, and actuator commands.
- `evaluator.c` computes plug/socket geometry and assigns tiered scores, based
  on 3-tier score rubric.
- `benchmark.c` runs trials, gathers path length/duration metrics, and writes 
  optional trace CSV files.
- `scripted_policy.c` implements a basic finite-state joint-space policy 
  (motion planning is hardcoded-coords, for now).
- `logging.c` handles console summaries and trace output.

## Scoring Model

Our scoring rubric for evaluating robot's performance in inserting cable:

- Tier 1: program/model validity. Awarded if program doesn't crash.
- Tier 2: motion quality, including duration, path length, smoothness 
  placeholder, and retry penalty.
- Tier 3: task success:
    - full insertion
    - partial insertion
    - proximity to socket

Full insertion is judged using plug-tip (TCP) position relative to the socket 
axis.

## Attribution

The MuJoCo robot/environment assets are derived from [Intrinsic's AI for Industry Challenge](https://www.intrinsic.ai/events/ai-for-industry-challenge)
hosted by Intrinsic, Google Deepmind, NVIDIA and Foxconn.

## Limitations

- The scripted policy is a simple hardcoded demonstration controller, not a
  learned (e.g., heuristics or ACT) policy.
- The current policy uses relative joint-space waypoints, not full Cartesian IK.
- Cable/contact physics have been simplified for a reliable demo.
- The replay viewer is Python-based; the benchmark/evaluator core is C.

## Side Note
I chose to build this robotic infrastructure for the C project from scratch to
close a specific gap in my robotics knowledge. Previously, I've participated in
2 major sim-to-real robotics competition (NVIDIA x Revel && Intrinsic's AIC),
this means I'm experienced in operating robotics simulation engines like 
Gazebo, NVIDIA Isaac Sim, MuJoCo using PyTorch. However, and during each 
competiton, the benchmark/evaluator infrastructure were always provided by
the hosts. This left a gap in my understanding, as I never had the opportunity
 to designing and building my own simulation environment from scratch. Because 
desining this infrastructure is a crucial part of teaching a robot via 
sim-to-real pipelines, I've hence decided to tackle this undertaking for this 
`C project`. This also gave me the excuse to dive deeply in using MuJoCo's C
API to additionally upskill.

Author: Shi Hao