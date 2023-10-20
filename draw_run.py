import pandas as pd
import graphviz

def real_graph(name):
    df = pd.read_csv(f'/Users/avazquez/av2atg/red/data/{name}_edge_list.csv')
    graph = graphviz.Digraph(format='png')
    graph.graph_attr.update({'rankdir': 'LR'})
    nodes = list(set(df.pred.to_list()).union(set(df.succ.to_list())))
    for node in nodes:
        graph.node(str(node), shape="point")
    for arc in df.itertuples():
        graph.edge(str(arc.pred), str(arc.succ))
#    graph.render(filename=f'/Users/avazquez/av2atg/red/data/real_{i}_graph',format='png')
    graph.render(filename=f'/Users/avazquez/Dropbox/submissions/dupsplit_fractal_2023/{name}_graph',format='eps')


#real_graph(3)
#real_graph(174)
#real_graph(168)
#real_graph("real_0")

real_graph("real_160")
real_graph("real_195")
#real_graph("dupsplit_q0.10_n100")
#real_graph("dupsplit_q0.30_n100")
#real_graph("dupsplit_q0.50_n100")

