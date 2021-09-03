import base64
import hashlib
import sys
from itertools import permutations


def sha256(text):
    sh = hashlib.sha256()
    sh.update(text)
    return sh.hexdigest()


UTF8 = 'utf-8'


if __name__ == '__main__':
    base = []
    for c in sys.argv[1]:
        base += c

    if len(sys.argv) > 2:
        line_show = int(sys.argv[2])
    else:
        line_show = -1
    #print(base)
    perm = list(permutations(base, len(base)))
    print(len(perm))
    count = 0
    for p in perm:
        text = ''.join(p)
        code = sha256(text.encode(UTF8))
        codes = code
        #print(text, len(code), code)
        for c in p:
            code = sha256(c.encode(UTF8))
            codes += code
            #print('\t\t', c, len(code), code)
        #print(len(codes), codes)
        codessha = sha256(codes.encode(UTF8))
        #print(len(codessha), codessha)
        result = base64.b64encode(str(codessha).encode(UTF8)).decode(UTF8)
        # print(codessha, result)
        msg = ''
        for i in range(0, len(codessha), 8):
            msg += codessha[i : i + 8] + ' '
        msg += '  '
        for i in range(0, len(result), 8):
            msg += result[i : i + 8] + ' '
        if count == line_show or line_show == -1:
            print('{:4} {}'.format(count, msg))
            if line_show != -1:
                break
        count += 1
