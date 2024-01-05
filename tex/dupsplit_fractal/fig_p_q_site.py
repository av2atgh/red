import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import linregress
plt.rcParams.update({'font.size': 16})

n_list = [1000,10000,100000,1000000]

def size_dependence(qs):
    df = pd.DataFrame()
    for n in n_list:
        df_ = pd.read_csv(f'/Users/avazquez/av2atg/red/data/dupsplit_site_percolation_duplication_rate_0.{qs}_nodes_{n}_nets_10_percolations_10.csv')
        df_['n'] = n
        df = pd.concat([df, df_])
    p_list = df.loc[df.p_site>0.2].p_site.unique().tolist()
    n_slope = []
    for p in p_list:
        df1 = df.loc[df.p_site==p]
        x = df1.n.to_numpy()
        y = df1.mean_max_component_size.to_numpy()
        slope, intercept, r, p, se = linregress(np.log(x), np.log(y))
        n_slope.append(slope)
    return p_list, n_slope

fig,ax = plt.subplots(3,2)

i = 0
for qs in [30,60,70]:
    for n in n_list:
        df = pd.read_csv(f'/Users/avazquez/av2atg/red/data/dupsplit_site_percolation_duplication_rate_0.{qs}_nodes_{n}_nets_10_percolations_10.csv')
        ax[i,0].plot(df.p_site,df.mean_max_component_size, label=f' N={n}')
    ax[i,0].set(yscale='log', xlabel=r'$p$', ylabel=r'$G$')
    ax[i,0].legend(loc = 'lower right', frameon = 0, handletextpad = 0, fontsize = 14)
    ax[i,0].set_title(r'A) $q=0.3$' if i==0 else r'B) $q=0.6$' if i==1 else r'C) $q=0.7$', x=-0.1, y=1.05)

    p,n = size_dependence(qs)
    ax[i,1].scatter(p,n)
    ax[i,1].plot([0.2,1],[0,0],'k--')
    ax[i,1].set(xlabel=r'$p$', ylabel=r'$dG/dN$')

    i += 1

#plt.subplots_adjust(bottom=0, top=1.5, hspace=0.3)
plt.subplots_adjust(top=2.5, bottom=0, left=0, right=2, hspace=0.35, wspace=0.3)
plt.savefig(f'/Users/avazquez/Dropbox/submissions/dupsplit_fractal_2023/fig_p_q_site.eps',bbox_inches='tight', facecolor='white', edgecolor='none', dpi=300)
plt.savefig(f'/Users/avazquez/Dropbox/submissions/dupsplit_fractal_2023/fig_p_q_site.pdf',bbox_inches='tight', facecolor='white', edgecolor='none', dpi=300)

