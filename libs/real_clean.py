import os
import pandas as pd
import numpy as np

has_data = []
warehouse = "/Users/avazquez/nl/data/S3/nl-raw-data-warehouse/project-schedules-explorers/"
projects = pd.read_csv("/Users/avazquez/nl/plethora/floppy/warehouse_early.csv")
output = "/Users/avazquez/av2atg/red/data"
for p in projects.itertuples():
    try:
        task = pd.read_csv(os.path.join(warehouse,p.tenant,p.schedule,p.version,"models/sanity","nl_san_task.csv"))
    except FileNotFoundError:
        has_data.append(False)
    else:
        has_data.append(True)
projects = projects.loc[np.array(has_data)].reset_index(drop=True)

#
#   remove LOE & WBS, save edge list
#
i=0;
for p in projects.itertuples():
    task = pd.read_csv(os.path.join(warehouse,p.tenant,p.schedule,p.version,"models/sanity","nl_san_task.csv"))
    task_rel = pd.read_csv(os.path.join(warehouse,p.tenant,p.schedule,p.version,"models/sanity","nl_san_task_rel.csv"))
    task = task.loc[~task.task_type.isin(["LOE","WBS"])]
    task_id_to_ix = dict(zip(task.task_id.values, task.index))
    task_rel["pred"] = task_rel["pred_task_id"].map(task_id_to_ix)
    task_rel["succ"] = task_rel["succ_task_id"].map(task_id_to_ix)
    task_rel = task_rel[["pred","succ"]].dropna()
    for key in ["pred","succ"]:
        task_rel[key] = task_rel[key].astype(int)
    task_rel.to_csv(os.path.join(output,f"real_{i}_edge_list.csv"), index=False)
    i += 1
