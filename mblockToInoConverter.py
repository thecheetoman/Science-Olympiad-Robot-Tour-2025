import os, zipfile, sys
#convert the .mblock file into a zip file
def convertZip():
  print("type name of the file including .mblock, ex Example.mblock\n")
  name = input(">  ")
  new = name.replace(".mblock", ".zip")
  if (name == "exit"):
    sys.exit()
  print(str(name))
  print(str(new))
  try:
    # Attempt to rename the file
    os.rename(name, new)
    print(f"File successfuly converted")
  except FileNotFoundError:
    print(f"Error: The file '{name}' was not found.")
  except OSError as e:
    print(f"Error renaming file: {e}")
  extractZip(new)
def extractZip(name):
  print(f"Extracting {name}...")
  try:
    with zipfile.ZipFile(name, 'r') as zip_ref:
      extract_folder = name.replace('.zip', '')
      zip_ref.extractall(extract_folder)
      print(f"Successfully extracted to '{extract_folder}' folder")
      print(f"Extracted files: {zip_ref.namelist()}")
      printMscratchJson(extract_folder)
  except FileNotFoundError:
    print(f"Error: The file '{name}' was not found.")
  except zipfile.BadZipFile:
    print(f"Error: '{name}' is not a valid zip file.")
  except Exception as e:
    print(f"Error extracting zip file: {e}")
    printMscratchJson(name)

def printMscratchJson(extract_folder):
  import json
  import shutil
  mscratch_path = os.path.join(extract_folder, 'mscratch.json')
  print(f"\n--- Extracted code ---")
  try:
    with open(mscratch_path, 'r') as f:
      data = json.load(f)
      # Navigate through the JSON structure to find arduinoc code
      for sprite_id, sprite_data in data.get('spritesState', {}).items():
        if 'codes' in sprite_data and 'arduinoc' in sprite_data['codes']:
          arduinoc_code = sprite_data['codes']['arduinoc']
          # Replace \r\n, \r, and \n with actual newlines
          formatted_code = arduinoc_code.replace('\\r\\n', '\n').replace('\\r', '\n').replace('\\n', '\n')
          print(formatted_code)
          # Save the code to a .ino file
          ino_filename = f"{extract_folder}.ino"
          saveToIno(formatted_code, ino_filename)
          break
      else:
        print("No arduinoc code found in mscratch.json")
    
    # Delete the extracted folder after processing
    try:
      shutil.rmtree(extract_folder)
      print(f"Deleted folder '{extract_folder}'")
    except Exception as e:
      print(f"Error deleting folder: {e}")
      
  except FileNotFoundError:
    print(f"Error: mscratch.json not found in '{extract_folder}'")
  except json.JSONDecodeError as e:
    print(f"Error parsing JSON: {e}")
  except Exception as e:
    print(f"Error reading mscratch.json: {e}")

def saveToIno(text, filename="output.ino"):
  try:
    with open(filename, 'w') as f:
      f.write(text)
    print(f"Successfully saved code to '{filename}'")
  except Exception as e:
    print(f"Error saving to file: {e}")

while True:
  convertZip()
