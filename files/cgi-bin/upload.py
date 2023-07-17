import sys
import os

def save_uploaded_file():
    content_type = os.environ.get("CONTENT_TYPE", "")
    if "multipart/form-data" in content_type:
        # Find the boundary that separates the form data parts
        boundary = content_type.split("=")[1]

        # Read the data from stdin until the boundary is found
        data = sys.stdin.buffer.read()
        parts = data.split(b"--" + boundary.encode())

        for part in parts:
            # Check if this part contains a file
            if b"filename" in part:
                # Extract the file data and other information
                filename_start = part.find(b'filename="') + len(b'filename="')
                filename_end = part.find(b'"', filename_start)
                filename = part[filename_start:filename_end].decode()

                data_start = part.find(b"\r\n\r\n") + len(b"\r\n\r\n")
                file_data = part[data_start:-2]  # -2 to remove the trailing \r\n

                # Save the file data to a local file
                with open(filename, "wb") as f:
                    f.write(file_data)

save_uploaded_file()
