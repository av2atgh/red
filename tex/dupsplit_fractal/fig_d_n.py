import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 16})
from scipy.stats import linregress

def collect(qs, nnets):
    n = []
    d = []
    s = []
    for N in [1000,3000,10000,30000,100000,300000,1000000]:
        try:
            df = pd.read_csv(f'/Users/avazquez/av2atg/red/data/dupsplit_q{qs}_n{N}_nnets{nnets}_df.csv')
        except FileNotFoundError:
            continue
        n.append(N)
        d.append(df.dmean[0])
        s.append(df.dsigma[0])
    return pd.DataFrame(dict(n=n, d=d, s=s))


fig,ax = plt.subplots(2,1)

qs = '0.30'
df = collect(qs, 100)
x = df.n.values
y = df.d.values
yerr = df.s.values
#ax[0].scatter(x, y, color='orange', label=r' $q=$'+f'{qs}')
ax[0].errorbar(x, y, yerr=yerr, marker='s', linestyle='', color='orange', label=r' $q=$'+f'{qs}')
slope, intercept, r, p, se = linregress(np.log(x), np.log(y))
x = [min(x), max(x)]
ax[0].plot(x,np.exp(intercept + slope*np.log(x)), 'k--', label=r' $N^\beta$, $\beta=$'+f'{slope:.2f}')
ax[0].set(xscale='log', yscale='log')

qs = '0.60'
df = collect(qs, 100)
x = df.n.values
y = df.d.values
yerr = df.s.values
ax[1].errorbar(x, y, yerr=yerr, marker='s', linestyle='', color='orange', label=r' $q=$'+f'{qs}')
slope, intercept, r, p, se = linregress(np.log(x), np.log(y))
x = [min(x), max(x)]
ax[1].plot(x,np.exp(intercept + slope*np.log(x)), 'k--', label=r' $N^\beta$, $\beta=$'+f'{slope:.2f}')
ax[1].set(xscale='log', yscale='log')

"""
qs = '0.60'
df = collect(qs, 10)
x = df.n.values
y = df.d.values
yerr = df.s.values
ax[1].errorbar(x, y, yerr=yerr, marker='s', linestyle='', color='orange', label=r' $q=$'+f'{qs}')
slope, intercept, r, p, se = linregress(np.log(x), y)
x = [min(x), max(x)]
ax[1].plot(x, intercept + slope*np.log(x), 'k--', label=r' $\alpha=$'+f'{slope:.2f}')
ax[1].set(xscale='log')
"""

for i in [0,1]:
    ax[i].set(xlabel=r'$N$', ylabel=r'$\langle d\rangle$')
    ax[i].legend(loc = 'lower right', frameon = 0, handletextpad = 0, fontsize = 14)
    ax[i].set_title(r'A) $q=0.3$' if i==0 else r'B) $q=0.6$', x=-0.1, y=1.05)

plt.subplots_adjust(bottom=0, top=1.5, hspace=0.3)
plt.savefig('/Users/avazquez/Dropbox/submissions/dupsplit_fractal_2023/fig_d_n.eps',bbox_inches='tight', facecolor='white', edgecolor='none', dpi=300)
plt.savefig('/Users/avazquez/Dropbox/submissions/dupsplit_fractal_2023/fig_d_n.pdf',bbox_inches='tight', facecolor='white', edgecolor='none', dpi=300)

