# Define the paths to your HTML, CSS, and JavaScript files
html_file_path = "index.html"
css_file_path = "style.css"
js_file_path = "script.js"

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
html_content = html_content.replace('<link rel="stylesheet" href="style.css">', f'<style>{css_content}</style>')

# Replace <script src="script.js"></script> with the JavaScript content
html_content = html_content.replace('<script src="script.js"></script>', f'<script>{js_content}</script>')

# Save the modified HTML content to a new file
output_file_path = "output.html"
with open(output_file_path, "w") as output_file:
    output_file.write(html_content)

print("HTML file with inline CSS and JavaScript has been created at", output_file_path)
