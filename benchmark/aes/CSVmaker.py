import os, glob

variables = ["sim_total_insn", "sim_cycle", "sim_CPI", "dl1.misses", "dl1.miss_rate", "total_power ", "avg_total_power_insn ", "total_dcache_access", "Access Time: 8.89694e-09"]
csvlines = [",,sim_total_insn, sim_cycle, sim_CPI, dl1.misses, dl1.miss_rate, total_power, avg_total_power_insn, total_dcache_access, Access Time: 8.89694e-09\n"]

for filename in glob.glob('*.txt'):
    line = filename + ","
    with open(os.path.join(os.getcwd(), filename), 'r') as f:
        lines = f.readlines()
    
    for j in variables:    
        for i in lines:
            if j in i:
                line = line + "," + i.replace(" ", "").replace("\n", "").replace(",", "").split("#")[0].split(j.replace(" ", ""))[1]
    line = line + "\n"
    csvlines.append(line)
    
with open('datatable.csv', 'w') as g:
    g.writelines(csvlines)