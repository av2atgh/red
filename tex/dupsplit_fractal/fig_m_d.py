import numpy as np
import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 16})
from scipy.stats import linregress

df3_4 = pd.read_csv("/Users/avazquez/av2atg/red/data/dupsplit_q0.30_n10000_nnets10_fractal.csv")
df3_5 = pd.read_csv("/Users/avazquez/av2atg/red/data/dupsplit_q0.30_n100000_nnets10_fractal.csv")
df3_6 = pd.read_csv("/Users/avazquez/av2atg/red/data/dupsplit_q0.30_n1000000_nnets10_fractal.csv")
df6_4 = pd.read_csv("/Users/avazquez/av2atg/red/data/dupsplit_q0.60_n10000_nnets1_fractal.csv")
df6_5 = pd.read_csv("/Users/avazquez/av2atg/red/data/dupsplit_q0.60_n100000_nnets1_fractal.csv")
df6_6 = pd.read_csv("/Users/avazquez/av2atg/red/data/dupsplit_q0.60_n1000000_nnets1_fractal.csv")

fig,ax = plt.subplots(2,1)

# q=0.3

dmin=1
dmax=100

df4 = df3_5
df5 = df3_4
df6 = df3_6
x=np.array([1,df6.d.max()])
ax[0].scatter(df4.d,df4.m,label=r'$N=10^4$')
ax[0].scatter(df5.d,df5.m,label=r'$N=10^5$')
ax[0].scatter(df6.d,df6.m,label=r'$N=10^6$')
ax[0].set(xscale='log', yscale='log')
df = df4
df1 = df.loc[(df.d>=dmin) & (df.d<=dmax)]
slope, intercept, r, p, se = linregress(np.log(df1.d.values), np.log(df1.m.values))
#ax[0].plot(x, np.exp(intercept + slope*np.log(x)), 'k--', label=f' $d_f$ ={slope:.2f}')
df=df5
df1 = df.loc[(df.d>=dmin) & (df.d<=dmax)]
slope, intercept, r, p, se = linregress(np.log(df1.d.values), np.log(df1.m.values))
#ax[0].plot(x, np.exp(intercept + slope*np.log(x)), 'k-.', label=f' $d_f$ ={slope:.2f}')
df=df6
df1 = df.loc[(df.d>=dmin) & (df.d<=dmax)]
slope, intercept, r, p, se = linregress(np.log(df1.d.values), np.log(df1.m.values))
#ax[0].plot(x, np.exp(intercept + slope*np.log(x)), 'k:', label=f' $d_f$ ={slope:.2f}')
ax[0].plot(x, np.exp(1 + np.log(x)/0.55), 'k--', label=r' $d_f = \alpha^{-1}$')

# q=0.6

dmin=2
dmax=4

df4 = df6_4
df5 = df6_5
df6 = df6_6
x=np.array([1,10])
ax[1].scatter(df4.d,df4.m, label=r'$N=10^4$')
ax[1].scatter(df5.d,df5.m, label=r'$N=10^5$')
ax[1].scatter(df6.d,df6.m, label=r'$N=10^6$')
ax[1].set(xscale='log', yscale='log')
df = df4
df1 = df.loc[(df.d>=dmin) & (df.d<=dmax)]
slope, intercept, r, p, se = linregress(np.log(df1.d.values), np.log(df1.m.values))
#ax[1].plot(x, np.exp(intercept + slope*np.log(x)), 'k--', label=f' $d_f$ ={slope:.2f}')
df = df5
df1 = df.loc[(df.d>=dmin) & (df.d<=dmax)]
slope, intercept, r, p, se = linregress(np.log(df1.d.values), np.log(df1.m.values))
#ax[1].plot(x, np.exp(intercept + slope*np.log(x)), 'k-.', label=f' $d_f$ ={slope:.2f}')
df = df6
df1 = df.loc[(df.d>=dmin) & (df.d<=dmax)]
slope, intercept, r, p, se = linregress(np.log(df1.d.values), np.log(df1.m.values))
#ax[1].plot(x, np.exp(intercept + slope*np.log(x)), 'k:', label=f' $d_f$ ={slope:.2f}')
ax[1].plot(x, np.exp(3 + np.log(x)/0.22), 'k--', label=r' $d_f = \alpha^{-1}$')

for i in [0,1]:
    ax[i].set(xlabel=r'$d$', ylabel=r'$M_d$')
    ax[i].legend(loc = 'lower right', frameon = 0, handletextpad = 0, fontsize = 14)
    ax[i].set_title(r'A) $q=0.3$' if i==0 else r'B) $q=0.6$', x=-0.1, y=1.05)

plt.subplots_adjust(bottom=0, top=1.5, hspace=0.3)
plt.savefig(f'/Users/avazquez/Dropbox/submissions/dupsplit_fractal_2023/fig_m_d.pdf',bbox_inches='tight', facecolor='white', edgecolor='none', dpi=300)

