#
#   script to generate figures
#

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 16})
from scipy.stats import linregress
from scipy.special import erf
from scipy.optimize import fsolve
from statistics import mode, mean
import warnings
warnings.filterwarnings("ignore")

colors = ["red", "blue", "green"]
markers = ["o", "s", "^"]
lines = ["--", "-.", ":"]
titles = ["A", "B", "C"]

#
#   log-binning
#

def loghis(x, base=1.2, density=True):
    stop_at = np.rint(np.log(max(x))/np.log(base)).astype(int)
    bins = np.logspace(start=0, stop=stop_at, num=stop_at+1, base=base)
    widths = (bins[1:] - bins[:-1])
    hist = np.histogram(x, bins=bins, density=density)
    x = bins[:-1]
    y = hist[0]
    return x[y>0], y[y>0]

#
#   law of activity delays
#

mu0 = 2.07
sigma0 = 1.20
g0 = -1.570
g1 = 0.596

def p_delay(x):
    return 1 /( 1 + np.exp( -g0 - g1*np.log(x) ) )

def LogNorm(x):
    return 0.5+0.5*erf(( np.log(x) - mu0 ) / (sigma0*np.sqrt(2)))

# sigma(n) scaling

def fb(n):
    def f(x):
        return 2*np.pi*(np.log(x))**2*np.exp((np.log(x))**2)-n**2
    return (fsolve(f, x0=n))[0]

#
#   duplication-split model
#

fig_name = "fig_model"

v = "duration"

fig,ax = plt.subplots(1,2)

Q="0.10"
NNODES=1000000

i = 0
for DMEAN in ["0"]:
    df = pd.read_csv(f"/Users/avazquez/av2atg/red/data/dupsplit_q{Q}_n{NNODES}_dmean{DMEAN}_i0_nodes.csv")
    for key in ["duration", "total_float"]:
        df[key]=df[key]*NNODES + int(key=="total_float")
    x, y = loghis(df[v].values, base=2)
    ax[0].scatter(x, y, marker=markers[i], c=colors[i])
    x1 = x[x>10]
    y1 = y[x>10]
    slope, intercept, r, p, se = linregress(np.log(x1), np.log(y1))
    ax[0].plot(x, np.exp(intercept + slope*np.log(x)), linestyle=lines[i], label=f' $\gamma_d$ ={-slope:.1f}', c=colors[i])
    i += 1
ax[0].set(xlabel=r"$d$",ylabel=r'$P(d)$', xscale='log', yscale='log')
ax[0].legend(loc = 'lower left', frameon = 0, handletextpad = 0, fontsize = 12)

v="total_float"

DMEAN=30
NNODES=1000000

i = 0
for Q in ["0.10", "0.30", "0.60"]:
    df = pd.read_csv(f"/Users/avazquez/av2atg/red/data/dupsplit_q{Q}_n{NNODES}_dmean{DMEAN}_i0_nodes.csv")
    for key in ["duration", "total_float"]:
        df[key]=df[key]*DMEAN*NNODES + int(key=="total_float")
    x, y = loghis(df[v].values, base=2)
    ax[1].scatter(x, y, marker=markers[i], c=colors[i], label=f"$q=${Q[:-1]}")
    x1 = x[1:-1]
    y1 = y[1:-1]    
    slope, intercept, r, p, se = linregress(np.log(x1), np.log(y1))
    ax[1].plot(x, np.exp(intercept + slope*np.log(x)), linestyle=lines[i], label=f' $\gamma_T$ ={-slope:.1f}', c=colors[i])
    i += 1
ax[1].set(xlabel=r"$T$",ylabel=r"$P(T)$", xscale='log', yscale='log')
ax[1].legend(loc = 'lower left', frameon = 0, handletextpad = 0, fontsize = 12)

plt.subplots_adjust(bottom=0, right=2, wspace=0.3)
plt.savefig(f'/Users/avazquez/Dropbox/submissions/dupsplit_2024/{fig_name}.pdf',bbox_inches='tight', facecolor='white', edgecolor='none', dpi=300)

fig_name = "fig_model_d1"

v="total_float"
fig,ax = plt.subplots()
DMEAN=30
NNODES=1000000
colors = ["red", "blue", "green", "cyan"]
markers = ["o", "s", "^"]
lines = ["--", "-.", ":"]
i = 0
for Q in ["0.10", "0.30", "0.60"]:
    df = pd.read_csv(f"/Users/avazquez/av2atg/red/data/dupsplit_constant_q{Q}_n{NNODES}_i0_nodes.csv")
    df["total_float"]=df["total_float"] + 1
    x, y = loghis(df[v].values, base=2)
    ax.scatter(x, y, marker=markers[i], c=colors[i], label=f"$q=${Q[:-1]}")
    x1 = x[1:-1]
    y1 = y[1:-1]    
    slope, intercept, r, p, se = linregress(np.log(x1), np.log(y1))
    ax.plot(x, np.exp(intercept + slope*np.log(x)), linestyle=lines[i], label=f' $\gamma_T$ ={-slope:.1f}', c=colors[i])
    i += 1
ax.set(xlabel=r"$T$",ylabel=r"$P(T)$", xscale='log', yscale='log')
ax.legend(loc = 'lower left', frameon = 0, handletextpad = 0, fontsize = 12)

plt.savefig(f'/Users/avazquez/Dropbox/submissions/dupsplit_2024/{fig_name}.pdf',bbox_inches='tight', facecolor='white', edgecolor='none', dpi=300)

#
#   construction projects
#

data = pd.read_csv("/Users/avazquez/av2atg/red/data/warehouse_data.csv")
projects = list(data.project_id.unique())

fig_name = "fig_data_d"

fig,ax = plt.subplots(1,3)

i = 0
for p in projects[:2]:
    df = data.loc[data.project_id==p].copy()
    v="target_duration_days"
    x, y = loghis(df[v].values, base=2)
    xmin = (np.sort(x))[-6]
    x1 = x[x>xmin]
    y1 = y[x>xmin]
    slope, intercept, r, p, se = linregress(np.log(x1), np.log(y1))
    ax[i].scatter(x, y, marker=markers[i], c=colors[i])
    ax[i].plot(x, np.exp(intercept + slope*np.log(x)), linestyle=lines[i], c=colors[i], label=f' $\gamma_d$ ={-slope:.1f}')
    ax[i].set(xlabel=r"$d$", ylabel=r"$P(d)$", xscale='log', yscale='log')
    ax[i].legend(loc = 'upper right', frameon = 0, handletextpad = 0, fontsize = 12)
    ax[i].set_title(f"{titles[i]}", x=-0.1)
    i += 1

slopes = []
for p in projects:
    df = data.loc[data.project_id==p].copy()
    v="target_duration_days"
    x, y = loghis(df[v].values, base=2)
    xmin = (np.sort(x))[-6]
    x1 = x[x>xmin]
    y1 = y[x>xmin]
    slope, intercept, r, p, se = linregress(np.log(x1), np.log(y1))
    slopes.append(-slope)
print(f"{len(slopes)} points", f"mean: {mean(slopes)}")
hist, bins = np.histogram(np.array(slopes), bins=7)
x = 0.5*(bins[1:]+bins[:-1])
y = hist
ax[2].bar(x,y, width=0.3, color="silver")
ax[2].set(xlabel=r"$\gamma_d$", ylabel="Number of Projects")
ax[2].set_title(f"{titles[2]}", x=-0.1)

plt.subplots_adjust(bottom=0, right=2, wspace=0.3)
plt.savefig(f'/Users/avazquez/Dropbox/submissions/dupsplit_2024/{fig_name}.pdf',bbox_inches='tight', facecolor='white', edgecolor='none', dpi=300)

fig_name = "fig_data_T"

fig,ax = plt.subplots(1,3)
i = 0
v="total_float_days"
for p in projects[:2]:
    df = data.loc[data.project_id==p].copy()  
    tf = 1 + np.maximum(0, df[v].values)
    x, y = loghis(tf, base=2)
    xmin = (np.sort(x))[-5]
    x1 = x[x>xmin]
    y1 = y[x>xmin]
    slope, intercept, r, p, se = linregress(np.log(x1), np.log(y1))
    ax[i].scatter(x, y, marker=markers[i], c=colors[i])
    ax[i].plot(x, np.exp(intercept + slope*np.log(x)), linestyle=lines[i], c=colors[i], label=f' $\gamma_T$ ={-slope:.1f}')
    ax[i].set(xlabel=r"$T$", ylabel=r"$P(T)$", xscale='log', yscale='log')
    ax[i].legend(loc = 'upper right', frameon = 0, handletextpad = 0, fontsize = 12)
    ax[i].set_title(f"{titles[i]}", x=-0.1)
    i += 1

slopes = []
for p in projects:
    df = data.loc[data.project_id==p].copy()
    tf = 1 + np.maximum(0, df[v].values)
    x, y = loghis(tf, base=2)
    if len(x)>=5:
        xmin = (np.sort(x))[-5]
        x1 = x[x>xmin]
        y1 = y[x>xmin]
        slope, intercept, r, p, se = linregress(np.log(x1), np.log(y1))
        slopes.append(-slope)
print(f"{len(slopes)} points", f"mean: {mean(slopes)}")
hist, bins = np.histogram(np.array(slopes), bins=7)
x = 0.5*(bins[1:]+bins[:-1])
y = hist
ax[2].bar(x,y, width=0.55, color="silver")
ax[2].set(xlabel=r"$\gamma_T$", ylabel="Number of Projects", xticks=np.arange(-1, 4, step=1))
ax[2].set_title(f"{titles[i]}", x=-0.1)

plt.subplots_adjust(bottom=0, right=2, wspace=0.3)
plt.savefig(f'/Users/avazquez/Dropbox/submissions/dupsplit_2024/{fig_name}.pdf',bbox_inches='tight', facecolor='white', edgecolor='none', dpi=300)

fig_name = "fig_Qz_D"

fig,ax = plt.subplots(1,3)
i = 0
z = np.linspace(1, 100*365, num=100)
for p in projects[:2]:
    df = data.loc[data.project_id==p].copy()
    T = np.maximum(0, df["total_float_days"].to_numpy())
    d = 1 + df["target_duration_days"].to_numpy()
    p = p_delay(d)
    r = (z[:,np.newaxis] - T[np.newaxis,:]) / d[np.newaxis,:]
    r = np.where(r>0, np.log(1 - p[np.newaxis,:] + p[np.newaxis,:] * LogNorm(r) ), 0)
    pz = 1 - np.exp( np.sum(r, axis=1) )
    ax[i].plot(z/365,pz, c="blue")
    p95_ = z[np.argmin(np.abs(pz-0.95))] / 365
    print(p95_)
    ax[i].plot([0,p95_,p95_], [0.95, 0.95,0], 'k--', label='P05')
    ax[i].set(xlabel=r"$z$ (years)", ylabel=r"$Q(z)$", xticks=[0,25,50,75,100])
    ax[i].legend(loc = 'upper right', frameon = 0, handletextpad = 0, fontsize = 12)
    ax[i].set_title(f"{titles[i]}", x=-0.1)
    i += 1

z = np.linspace(1, 500*365, num=100)
p95 = []
qin = []
qou = []
pro = []
for p in projects:
    df = data.loc[data.project_id==p].copy()
    p_data = df.iloc[0]
    T = np.maximum(0, df["total_float_days"].to_numpy())
    d = 1 + df["target_duration_days"].to_numpy()
    r = (z[:,np.newaxis] + T[np.newaxis,:]) / d[np.newaxis,:]
    p = p_delay(d)
    r = (z[:,np.newaxis] - T[np.newaxis,:]) / d[np.newaxis,:]
    r = np.where(r>0, np.log(1 - p[np.newaxis,:] + p[np.newaxis,:] * LogNorm(r) ), 0)
    pz = 1 - np.exp( np.sum(r, axis=1) )
    p95_ = z[np.argmin(np.abs(pz-0.95))] / 365
    p95.append(p95_)
    qin.append(p_data["qin"])
    qou.append(p_data["qou"])
    pro.append(p_data["project_name"])
hist, bins = np.histogram(np.array(p95), bins=7)
x = 0.5*(bins[1:]+bins[:-1])
y = hist
x0 = x[np.argmax(y)]
print(f'{int(100*sum(y[x>x0])/sum(y))}%')
slope, intercept, r, p, se = linregress(np.log(x), np.log(y))
print(slope)
ax[2].bar(x,y, width=0.1*max(p95), color="silver")
x = np.linspace(20, max(x), 100)
ax[i].plot(x, np.exp(intercept + slope*np.log(x)), linestyle="-", c="black", label='$P05^{-1.5}$')
ax[2].set(xlabel="P05 (years)", ylabel="Number of Projects")
ax[2].legend(loc = 'upper right', frameon = 0, handletextpad = 0, fontsize = 12)
ax[2].set_title(f"{titles[i]}", x=-0.1)
    
plt.subplots_adjust(bottom=0, right=2, wspace=0.3)
plt.savefig(f'/Users/avazquez/Dropbox/submissions/dupsplit_2024/{fig_name}.pdf',bbox_inches='tight', facecolor='white', edgecolor='none', dpi=300)

fig_name = "fig_Qz_exp"

fig,ax = plt.subplots(1,3)

z = np.linspace(1, 100*365, num=100)

i=0
for project in projects[:3]:

    df = data.loc[data.project_id==project].copy()
    
    # baseline
    T = np.maximum(0, df["total_float_days"].to_numpy())
    d = 1 + df["target_duration_days"].to_numpy()
    r = (z[:,np.newaxis] - T[np.newaxis,:]) / d[np.newaxis,:]
    print(r.shape)
    p_ = p_delay(d)
    r = np.where(r>0, np.log(1 - p_[np.newaxis,:] + p_[np.newaxis,:] * LogNorm(r) ), 0)
    pz = 1 - np.exp( np.sum(r, axis=1) )
    ax[i].plot(z/365,pz, c="blue", label="original")
    print(f"Maximum total float: {T.max()}")

    # d exp
    dmean = d.mean()
    d1 = -dmean * np.log(np.random.random(d.size))
    r = (z[:,np.newaxis] - T[np.newaxis,:]) / d1[np.newaxis,:]
    p_ = p_delay(d1)
    r = np.where(r>0, np.log(1 - p_[np.newaxis,:] + p_[np.newaxis,:] * LogNorm(r) ), 0)
    pz = 1 - np.exp( np.sum(r, axis=1) )
    ax[i].plot(z/365,pz, c="green", label=r"$P(d)\sim e^{-d/\langle d\rangle}$", linestyle=":")
    ax[i].set(xlabel=r"$z$ (years)", ylabel=r"$Q(z)$", xticks=[0,25,50,75,100])
    ax[i].legend(loc = 'upper right', frameon = 0, handletextpad = 0, fontsize = 12)
    ax[i].set_title(f"{titles[i]}", x=-0.1)

    # T=0
    d1 = d
    Tmean = T.mean()
    T1 = np.zeros_like(T)
    r = (z[:,np.newaxis] - T1[np.newaxis,:]) / d1[np.newaxis,:]
    p_ = p_delay(d1)
    r = np.where(r>0, np.log(1 - p_[np.newaxis,:] + p_[np.newaxis,:] * LogNorm(r) ), 0)
    pz = 1 - np.exp( np.sum(r, axis=1) )
    ax[i].plot(z/365,pz, c="red", label=r"$T_i=0$", linestyle="--")
    ax[i].set(xlabel=r"$z$ (years)", ylabel=r"$Q(z)$", xticks=[0,25,50,75,100])
    ax[i].legend(loc = 'upper right', frameon = 0, handletextpad = 0, fontsize = 12)
    ax[i].set_title(f"{titles[i]}", x=-0.1)

    # T exp
    T1 = np.zeros_like(T)
    T1 = np.random.permutation(T)
    r = (z[:,np.newaxis] - T1[np.newaxis,:]) / d[np.newaxis,:]
    p_ = p_delay(d)
    r = np.where(r>0, np.log(1 - p_[np.newaxis,:] + p_[np.newaxis,:] * LogNorm(r) ), 0)
    pz = 1 - np.exp( np.sum(r, axis=1) )
    #ax[i].plot(z/365,pz, c="red", label=r"$T_i=0$", linestyle="--")
    #ax[i].set(xlabel=r"$z$ (years)", ylabel=r"$Q(z)$", xticks=[0,25,50,75,100])
    #ax[i].legend(loc = 'upper right', frameon = 0, handletextpad = 0, fontsize = 12)
    #ax[i].set_title(f"{titles[i]}", x=-0.1)
    
    i += 1
    
plt.subplots_adjust(bottom=0, right=2, wspace=0.3)
plt.savefig(f'/Users/avazquez/Dropbox/submissions/dupsplit_2024/{fig_name}.pdf',bbox_inches='tight', facecolor='white', edgecolor='none', dpi=300)

#
#   bn vs n, lognormal
#

fig_name = "fig_zc"

fig, ax = plt.subplots(1,3)

n = [10,100,1000,10000,100000]
b = []
for i in n:
    b.append(fb(i))

i=0
sigma = b / np.log(b)
ax[i].scatter(n, sigma)
slope, intercept, r, p, se = linregress(np.log(n), np.log(sigma))
print(slope, np.exp(intercept))
ax[i].plot(n, np.exp(intercept + slope*np.log(n)), linestyle="--", c="black", label=r'$n^{0.20}$')
ax[i].set(xlabel=r"$n$", ylabel=r"$\sigma(n)$", xscale='log', yscale='log')
ax[i].legend(loc = 'upper left', frameon = 0, handletextpad = 0, fontsize = 12)
ax[i].set_title(f"{titles[i]}", x=-0.1)

# z / D

z = np.linspace(1, 500*365, num=100)
p95 = []
for project in projects:
    df = data.loc[data.project_id==project].copy()
    p_data = df.iloc[0]
    T = np.maximum(0, df["total_float_days"].to_numpy())
    d = 1 + df["target_duration_days"].to_numpy()
    p_ =  p_delay(d)
    r = (z[:,np.newaxis] - T[np.newaxis,:]) / d[np.newaxis,:]
    r = np.where(r>0, np.log(1 - p_[np.newaxis,:] + p_[np.newaxis,:] * LogNorm(r)), 0)
    pz = 1 - np.exp( np.sum(r, axis=1) )
    p95_ = z[np.argmin(np.abs(pz-0.95))]
    p95.append(p95_ / p_data["project_duration"])
    
i=1
hist, bins = np.histogram(np.array(p95), bins=7)
x = 0.5*(bins[1:]+bins[:-1])
y = hist
x0 = x[np.argmax(y)]
print(f'{int(100*sum(y[x>x0])/sum(y))}%')
ax[i].bar(x,y, width=0.1*max(x), color="silver")
slope, intercept, r, p, se = linregress(np.log(x[(y>0) & (x>5)]), np.log(y[(y>0) & (x>5)]))
print(slope)
x = np.linspace(8, max(x), 100)
ax[i].plot(x, np.exp(intercept + slope*np.log(x)), linestyle="-", c="black", label='$x^{-2.6}$')
ax[i].set(xlabel="P05 / D", ylabel="Number of Projects")
ax[i].legend(loc = 'upper right', frameon = 0, handletextpad = 0, fontsize = 12)
ax[i].set_title(f"{titles[i]}", x=-0.1)

# z /zc

z = np.linspace(1, 500*365, num=100)
p95 = []
for project in projects:
    df = data.loc[data.project_id==project].copy()
    p_data = df.iloc[0]
    T = np.maximum(0, df["total_float_days"].to_numpy())
    d = 1 + df["target_duration_days"].to_numpy()
    dmax = d.max()
    N = d.size
    # calculate gamma_d
    x, y = loghis(d-1, base=2)
    xmin = (np.sort(x))[-6]
    x1 = x[x>xmin]
    y1 = y[x>xmin]
    slope, intercept, r, p, se = linregress(np.log(x1), np.log(y1))
    gamma_d = -slope
    # calculate zc
    zc = N**0.2*dmax**(1+0.2*(1-gamma_d))
    # calculate P05
    r = (z[:,np.newaxis] + T[np.newaxis,:]) / d[np.newaxis,:]
    p =  p_delay(d)
    r = (z[:,np.newaxis] - T[np.newaxis,:]) / d[np.newaxis,:]
    r = np.where(r>0, np.log(1 - p[np.newaxis,:] + p[np.newaxis,:] * LogNorm(r) ), 0)
    pz = 1 - np.exp( np.sum(r, axis=1) )
    p95_ = z[np.argmin(np.abs(pz-0.95))]
    p95.append(p95_ / zc)
    
i=2
hist, bins = np.histogram(np.array(p95), bins=7)
x = 0.5*(bins[1:]+bins[:-1])
y = hist
x0 = x[np.argmax(y)]
print(f'{int(100*sum(y[x>x0])/sum(y))}%')
ax[i].bar(x,y, width=0.1*max(x), color="silver")
slope, intercept, r, p, se = linregress(np.log(x[x>25]), np.log(y[x>25]))
print(slope)
x = np.linspace(30, max(x), 100)
ax[i].plot(x, np.exp(intercept + slope*np.log(x)), linestyle="-", c="black", label='$x^{-2.7}$')
ax[i].set(xlabel=r"P05 / $z_c$", ylabel="Number of Projects")
ax[i].legend(loc = 'upper right', frameon = 0, handletextpad = 0, fontsize = 12)
ax[i].set_title(f"{titles[i]}", x=-0.1)

plt.subplots_adjust(bottom=0, right=2, wspace=0.3)
plt.savefig(f'/Users/avazquez/Dropbox/submissions/dupsplit_2024/{fig_name}.pdf',bbox_inches='tight', facecolor='white', edgecolor='none', dpi=300)

