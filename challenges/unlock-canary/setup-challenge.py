#!/usr/bin/env python3

import secrets
import string
import json

def main():
    # ========= Create flag =========
    rand_char = ''.join(secrets.choice(string.ascii_letters + string.digits) for _ in range(5))

    new_flag = "picoCTF{n3vEr_tRu2t_us3r_coNtr0Ii3d_f0rmaT_str1ng" + rand_char + "}"
    
    with open("app/flag.txt", "w") as f:
        f.write(new_flag)

    # ========= Create canary =========
    canary_value = secrets.randbits(64)
    with open("app/canary.txt", "w") as f:
        f.write(str(canary_value))

    # ========= Create and update metadata.json =========
    metadata = {}
    metadata['flag'] = str(new_flag)
    json_metadata = json.dumps(metadata)
    
    with open("/challenge/metadata.json", "w") as f:
        f.write(json_metadata)

if __name__ == "__main__":
    main()