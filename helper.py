def transfer_map(m, 
    transfer_key = lambda x : x if x <= 3 else x - 1,
    transfer_value = lambda x : 'soccer & football' if x == "soccer" else x,
    drop_kv = lambda k,v : True if v == "football" else False):
    new_m = {}
    for k, v in m.items():
        if drop_kv(k, v):
            continue
        new_m[transfer_value(v)] = transfer_key(k)
    return new_m