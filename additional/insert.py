import random

image_names = ['girl.gif', 'boy.png', 'eye.jpg', 'index.jpg']
read_file = open('meme_template.txt', 'r')
write_file = open('insert.sh', 'w')
lines = [line.strip(" \n") for line in read_file]

i = 0
while i + 1 < len(lines) and i < len(lines):
    image_idx = random.randint(0, 3)
    name = image_names[image_idx]
    url = "/img/" + name
    insert = "sqlite3 meme.db \"INSERT INTO meme_info (top_text, bottom_text, image_name, image_url) VALUES ('{}', '{}', '{}', '{}');\"\n".format(lines[i], lines[i+1], name, url)
    write_file.write(insert)
    i += 2

read_file.close()
write_file.close()
