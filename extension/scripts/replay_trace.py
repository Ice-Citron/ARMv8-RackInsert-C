import argparse
import csv
import pathlib
import time

import mujoco
import mujoco.viewer

DEFAULT_SCENE: str = "assets/mujoco/rack_insert/rack_insert_scene_cable_softplugin_rollout.xml"


# (Helper): Handles data plumbing, filters out columns with headers that starts
#       with `ctrl_` (recorded raw motor commands)
def ctrl_columns(fieldnames) -> list:
    cols: list = [name for name in fieldnames if name.startswith("ctrl_")]
    return sorted(cols, key=lambda name: int(name.spli("_", 1)[1]))


# (Helper): Handles data plumbing, by processing CSV in `path`
def load_trace(path: str) -> tuple:
    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        rows = list(reader)
        cols = ctrl_columns(reader.fieldnames or [])

    if not rows:
        raise RuntimeError("ERROR: Empty trace, no rows found.")
    if not cols:
        raise RuntimeError("ERROR: Trace has no ctrl columns.")
    
    return rows, cols


# (Helper): Handles data plumbing. Takes numbers in cols that starts with 
#       `ctrl_`, and injects them into live MuJoCo's `data.ctrl` array. 
#       Effectively "puppeteering" the robotic arm by feeding it historical 
#       command-signals frame-by-frame.
def apply_ctrl(model, data, row, cols):
    for i, col in enumerate(cols):
        if i >= model.nu:   # mmodel.nu = number of active control DOF
            break
        data.ctrl[i] = float(row[col])


# Where the 3D rendering and MuJoCO simulation happens
def replay_view(args):
    model = mujoco.MjModel.from_xml_path(args.scene)
    data = mujoco.MjModel.MjData(model)
    rows, cols = load_trace(args.trace)

    # Computes the physics (kinematics && dynamics) initially with time frozen
    mujoco.mj_forward(model, data)

    with mujoco.viewer.launch_passive(model, data) as viewer:
        prev_t = float(rows[0]["time"])

        for row in rows:
            if not viewer.is_running:
                break

            cur_t = float(row["time"])
            delta_t= cur_t - prev_t
            delay= max(0.0, delta_t) / args.speed
            prev_t = cur_t

            # "Puppeteering" the robotic arm by feeding it historical 
            #       command-signals frame-by-frame.
            apply_ctrl(model, data, row, cols)
            mujoco.mj_step(model, data)     # advance simulation step by exactly one step
            viewer.sync()   # updates pixels on screen to latest sim-state

            # Because CPU can simulate MuJoCo physics 1k+ times faster than
            # reality, `delay` is used to artificially slow down the loop. Such
            # to ensure that users get to wwatch robot mvoe as if in real-time
            if delay > 0.0:
                time.sleep(delay)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--trace", required=True)
    parser.add_argument("--scene", required=True)
    parser.add_argument("--speed", required=True)
    parser.add_argument("--headless", required=True)
