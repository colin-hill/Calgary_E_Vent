line = "Arduino Time:11005"
splits = line.split(":")
print(splits)
new_line = line.ljust(15)
new_line = new_line + "2"
print(new_line)