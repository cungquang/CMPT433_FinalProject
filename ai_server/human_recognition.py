import threading
import socket
import cv2

SERVER_IP = '127.0.0.1'
SERVER_PORT = 5555

PIXEL_CONVERSION_RATE = 0.02645


#########################
#                       #
#   Human Recognition   #
#                       #
#########################

#Function for debugging:
def debug_detect_human(image_path):
    # Load pre-trained human detector
    human_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_fullbody.xml')

    # Load image
    image = cv2.imread(image_path)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Detect humans in the image
    humans = human_cascade.detectMultiScale(gray, 1.1, 4)

    if len(humans) > 0:
        # Get the coordinates of the first detected human
        x, y, w, h = humans[0]

        # Draw rectangle around the detected human
        cv2.rectangle(image, (x, y), (x + w, y + h), (255, 0, 0), 2)

        # Calculate the center of the detected human
        human_center_x = x + w // 2
        human_center_y = y + h // 2

        # Calculate the center of the image
        image_center_x = image.shape[1] // 2
        image_center_y = image.shape[0] // 2

        # Calculate the distance from human to the center of the image
        distance = np.sqrt((human_center_x - image_center_x)**2 + (human_center_y - image_center_y)**2)

        # Display distance on the image
        cv2.putText(image, f"Distance: {image_center_x:.2f} pixels", (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)

        # Show the image with the detected human and distance
        cv2.imshow("Human Detection", image)
        cv2.waitKey(0)
        cv2.destroyAllWindows()
    else:
        print("No human detected in the image.")


#Source: ChatGPT
def detect_human(image_path):
    human_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_fullbody.xml')
    image = cv2.imread(image_path)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    humans = human_cascade.detectMultiScale(gray, 1.1, 4)

    if len(humans) > 0:
        x, y, w, h = humans[0]
        human_center_x = x + w // 2
        return human_center_x
    else:
        #print("No human detected in the image.")
        return None

def get_gap_from_center(human_center_x, image_path):
    image = cv2.imread(image_path)
    image_width = image.shape[1]
    center = image_width / 2
    gap = human_center_x - center

    return gap

def identify_human_position(image_path):
    human_center_x = detect_human(image_path)

    #If found human
    if human_center_x is not None:
        #Return position: left -> negative; center -> 0; right -> positive
        return get_gap_from_center(human_center_x, image_path)
    
    else:
        #No human found OR unable to identify human
        return None

#on average 1 pixel = 0.02645 cm
def convert_pixel_cm(pixel):
    return round(pixel*PIXEL_CONVERSION_RATE)



#########################
#                       #
#      TCP Server       #
#                       #
#########################


def handle_client(client_socket):
    while True:
        request = client_socket.recv(1024).decode('utf-8')

        if not request:
            break

        #Handle message:
        if request == "shutdown":
            break

        response = f"From: {request}"

        client_socket.send(response.encode('utf-8'))

    #close connection
    client_socket.close()


def start_tcp_server(host, port):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Bind the socket to the address and port
    server_socket.bind((host, port))

    # Listen for incoming connections
    server_socket.listen(1)
    print(f"Server is listening on {host}:{port}")

    try:
        while True:
            # Accept incoming connections
            client_socket, address = server_socket.accept()
            print(f"[*] Accepted connection from {address[0]}:{address[1]}")

            client_thread = threading.Thread(target=handle_client, args=(client_socket,))
            client_thread.start()

    except:
        print("\n[*] Server shutting down.")
        server_socket.close()



def main():
    # Example usage
    # image_path = './pictures/test1.JPG'
    # human_position_pixel = identify_human_position(image_path)
    # human_position_cm = convert_pixel_cm(human_position_pixel)
    # print(human_position_cm)

    start_tcp_server(SERVER_IP, SERVER_PORT)

if __name__ == "__main__":
    main()