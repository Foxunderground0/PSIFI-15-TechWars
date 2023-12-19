import gzip
import io

# Define the paths to your HTML, CSS, and JavaScript files
html_file_path = "Web\index.html"
css_file_path = "Web\style.css"
js_file_path = "Web\script.js"

# Read the content of the HTML file
with open(html_file_path, "r") as html_file:
    html_content = html_file.read()

# Read the content of the CSS file
with open(css_file_path, "r") as css_file:
    css_content = css_file.read()

# Read the content of the JavaScript file
with open(js_file_path, "r") as js_file:
    js_content = js_file.read()

# Replace <link rel="stylesheet" href="style.css"> with the CSS content
html_content = html_content.replace(
    '<link rel="stylesheet" href="style.css">', f'<style>{css_content}</style>')

# Replace <script src="script.js"></script> with the JavaScript content
html_content = html_content.replace(
    '<script src="script.js"></script>', f'<script>{js_content}</script>')

# Gzip ultra compress the HTML content with maximum compression level (level=9)
html_content_bytes = html_content.encode('utf-8')
gzip_html_content = gzip.compress(html_content_bytes, compresslevel=9)

# Save the compressed HTML content as a .gz file
output_file_path = "Main\data\index.html.gz"
with open(output_file_path, "wb") as output_file:
    output_file.write(gzip_html_content)

print(
    f"Gzipped HTML file with inline CSS and JavaScript has been created as '{output_file_path}'")
