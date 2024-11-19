from PIL import Image

# 이미지 파일 열기
img = Image.open("map1.png").convert("RGB")
width, height = img.size

# 결과를 저장할 이중배열 생성
pixel_values = []

for y in range(height):
    row = []
    for x in range(width):
        r, g, b = img.getpixel((x, y))

        if (r, g, b) == (0, 0, 0):       # 벽
            row.append(0)
        elif (r, g, b) == (255, 255, 255):  # 길
            row.append(1)
        elif (r, g, b) == (255, 0, 0):    # 시작
            row.append(2)
        elif (r, g, b) == (0, 255, 0):    # 종료
            row.append(3)
        else:
            row.append(None)  # 처리하지 않은 색상은 None

    pixel_values.append(row)

# 결과 저장
with open('map.dat', 'w') as f:
    f.write(f"{height} {width}\n")  # 높이와 너비를 기록
    for row in pixel_values:
        f.write(" ".join(str(cell) for cell in row) + "\n")
