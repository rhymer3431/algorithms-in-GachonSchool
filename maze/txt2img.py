from PIL import Image


# 예시로 사용할 이중 배열 (픽셀 값)
with open('result.dat','r') as f:
    domain = int(f.readline())
    print(domain)
    table = []
    for l in range(domain):
        line = f.readline()
        ls = line.split(' ')
        for i in range(len(ls)):
            ls[i] = int(ls[i])
        table.append(ls)
        # 새로운 이미지 생성 (모드 'RGB', 크기 domain x domain)
    img = Image.new("RGB", (domain, domain))
    
    # 픽셀값을 배열에서 하나씩 꺼내서 설정
    for y in range(domain):
        for x in range(domain):
            if table[y][x] == 1:
                img.putpixel((x, y), (255, 255, 255))  # 흰색
            elif table[y][x] == 0:
                img.putpixel((x, y), (0, 0, 0))        # 검정색
            elif table[y][x] == 2:
                img.putpixel((x, y), (255, 0, 0))      # 빨강
            elif table[y][x] == 3:
                img.putpixel((x, y), (0, 255, 0))      # 초록
            elif table[y][x] == 7:
                img.putpixel((x, y), (0, 0, 255))      # 파랑

    # 이미지 저장
    img.save("result.png")


