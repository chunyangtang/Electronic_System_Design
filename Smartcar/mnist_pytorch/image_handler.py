import pygame

def pygame_interactive() -> list:

    canvas_colors = []
    # Initialize pygame
    pygame.init()

    # Set the dimensions of the canvas
    canvas_width = 28
    canvas_height = 28

    # Set the size of each pixel
    pixel_size = 13  # 20

    # Set the colors
    black = (0, 0, 0)
    white = (255, 255, 255)
    gray = (128, 128, 128)

    # Create a blank canvas
    canvas = [[black] * canvas_width for _ in range(canvas_height)]

    # Set up the window
    window_width = canvas_width * pixel_size
    window_height = canvas_height * pixel_size + 50  # Add space for buttons
    window = pygame.display.set_mode((window_width, window_height))
    pygame.display.set_caption("Draw your number on the canvas")

    # Set up the font
    font = pygame.font.Font(None, 24)

    # Set up the buttons
    cancel_button = pygame.Rect(10, window_height - 40, 100, 30)
    submit_button = pygame.Rect(window_width - 110, window_height - 40, 100, 30)

    # Main loop
    running = True
    drawing = False
    submit_clicked = False
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1:
                    if cancel_button.collidepoint(event.pos):
                        # Clear the canvas if cancel button is clicked
                        canvas = [[black] * canvas_width for _ in range(canvas_height)]
                    elif submit_button.collidepoint(event.pos):
                        # Set submit_clicked to True when submit button is clicked
                        submit_clicked = True
                    else:
                        drawing = True
            elif event.type == pygame.MOUSEBUTTONUP:
                if event.button == 1:
                    drawing = False
            elif event.type == pygame.MOUSEMOTION and drawing:
                x, y = pygame.mouse.get_pos()
                pixel_x = x // pixel_size
                pixel_y = y // pixel_size

                # Gradually darken the pixels as you move the mouse
                for i in range(-1, 1):
                    for j in range(-1, 1):
                        nx = pixel_x + i
                        ny = pixel_y + j
                        if 0 <= nx < canvas_width and 0 <= ny < canvas_height:
                            alpha = 0.5 - (abs(i) + abs(j)) * 0.2  # Adjust the alpha value for softer effect
                            canvas[ny][nx] = (
                                int(canvas[ny][nx][0] * (1 - alpha) + white[0] * alpha),
                                int(canvas[ny][nx][1] * (1 - alpha) + white[1] * alpha),
                                int(canvas[ny][nx][2] * (1 - alpha) + white[2] * alpha)
                            )

        # Draw the canvas
        for y in range(canvas_height):
            for x in range(canvas_width):
                pygame.draw.rect(window, canvas[y][x], (x * pixel_size, y * pixel_size, pixel_size, pixel_size))

        # Draw the buttons
        pygame.draw.rect(window, gray, cancel_button)
        pygame.draw.rect(window, gray, submit_button)

        # Draw the button labels
        cancel_text = font.render("Clear", True, white)
        submit_text = font.render("Submit", True, white)
        window.blit(cancel_text, (cancel_button.x + 10, cancel_button.y + 5))
        window.blit(submit_text, (submit_button.x + 10, submit_button.y + 5))

        # Check if submit button is clicked
        if submit_clicked:
            # Convert the canvas colors to a list of integers
            canvas_colors = []
            for row in canvas:
                for color in row:
                    grayscale = sum(color) // 3  # Calculate grayscale value
                    canvas_colors.append(grayscale)

            submit_clicked = False  # Reset submit_clicked flag
            running = False  # Stop the main loop

        # Update the display
        pygame.display.update()

    # Quit pygame
    pygame.quit()

    return canvas_colors