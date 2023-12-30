import gzip
import io

# Define the paths to your HTML, CSS, and JavaScript files
html_file_path = "Challenges\JS Game\game.html"

# Read the content of the HTML file
with open(html_file_path, "r") as html_file:
    html_content = html_file.read()

# Gzip ultra compress the HTML content with maximum compression level (level=9)
html_content_bytes = html_content.encode('utf-8')
gzip_html_content = gzip.compress(html_content_bytes, compresslevel=9)

# Save the compressed HTML content as a .gz file
output_file_path = "Main\data\game.html.gz"
with open(output_file_path, "wb") as output_file:
    output_file.write(gzip_html_content)

print(
    f"Gzipped Game HTML file '{output_file_path}'")
