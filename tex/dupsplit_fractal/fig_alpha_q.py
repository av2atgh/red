import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 16})
from scipy.stats import linregress

def collect(qs, nnets, nnets1=1):
    n_ = [1000,10000,100000,1000000]
    n = []
    d = []
    for nnodes in n_:
        try:
            df = pd.read_csv(f'/Users/avazquez/av2atg/red/data/dupsplit_q{qs}_n{nnodes}_nnets{nnets}_df.csv')
        except FileNotFoundError:
            try:
                df = pd.read_csv(f'/Users/avazquez/av2atg/red/data/dupsplit_q{qs}_n{nnodes}_nnets{nnets1}_df.csv')
            except FileNotFoundError:
                continue
        n.append(nnodes)
        d.append(df.dmean[0])
    x = np.array(n)
    y = np.array(d)
    slope, intercept, r, p, se = linregress(np.log(x), np.log(y))
    return slope

q = []
alpha = []
nnets = 10
#for i in range(1,10):
for i in range(1,8):
    qs = f'0.{i}0'
    slope = collect(qs, nnets)
    q.append(float(qs))
    alpha.append(slope)

x = np.array(q)
y = np.array(alpha)

fig,ax = plt.subplots()
ax.scatter(x,y)
ax.scatter([0,1],[1,0])
ax.plot([0,1], [1,0], 'k--', label=r'$q$')
ax.set(xlabel=r'$q$', ylabel=r'$\beta$')
#    ax[i].legend(loc = 'lower right', frameon = 0, handletextpad = 0, fontsize = 14)
#    ax[i].set_title(r'A) $q=0.3$' if i==0 else r'B) $q=0.6$', x=-0.1, y=1.05)

#plt.subplots_adjust(bottom=0, top=1.5, hspace=0.3)
plt.savefig('/Users/avazquez/Dropbox/submissions/dupsplit_fractal_2023/fig_alpha_q.eps',bbox_inches='tight', facecolor='white', edgecolor='none', dpi=300)
plt.savefig('/Users/avazquez/Dropbox/submissions/dupsplit_fractal_2023/fig_alpha_q.pdf',bbox_inches='tight', facecolor='white', edgecolor='none', dpi=300)

