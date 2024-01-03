import gzip


def remove_comments(html_content):
    start_comment = "<!--"
    end_comment = "-->"

    while start_comment in html_content and end_comment in html_content:
        start_index = html_content.find(start_comment)
        end_index = html_content.find(end_comment) + len(end_comment)

        html_content = html_content[:start_index] + html_content[end_index:]

    return html_content


# Define the paths to your HTML, CSS, and JavaScript files
html_file_path = "Challenges\Web Server HTML Login\login.html"

# Read the content of the HTML file
with open(html_file_path, "r") as html_file:
    html_content = html_file.read()

# Remove comments from HTML content
html_content = remove_comments(html_content)

# Gzip ultra compress the HTML content with maximum compression level (level=9)
html_content_bytes = html_content.encode('utf-8')
gzip_html_content = gzip.compress(html_content_bytes, compresslevel=9)

# Save the compressed HTML content as a .gz file
output_file_path = "Main\data\login.html.gz"
with open(output_file_path, "wb") as output_file:
    output_file.write(gzip_html_content)

print(f"Gzipped Game HTML file '{output_file_path}'")
