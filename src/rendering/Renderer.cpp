#include "../core/Game.hpp"
#include <cmath>

constexpr float PI = 3.14159265358979323846f;
constexpr float E = 2.71828182845904523536f;
constexpr int NOISE_SIZE = 32;
constexpr int NOISE_MASK = NOISE_SIZE - 1;
constexpr float NOISE_SCALE = 2.5f;

// Static noise pattern array
static float noisePatternArray[NOISE_SIZE][NOISE_SIZE];
static bool noiseInitialized = false;

// Smooth noise function
float Game::smoothNoise(float x, float y) {
    const float x_scaled = x * 0.15f;
    const float y_scaled = y * 0.2f;
    const float xy_sum = (x + y) * 0.1f;
    const float dist = std::sqrt(x*x + y*y) * 0.1f;
    
    return (std::sin(x_scaled + y_scaled) * 0.5f +
            std::cos(x_scaled * 0.8f - y_scaled * 0.7f) * 0.3f +
            std::sin(xy_sum) * 0.2f +
            std::cos(dist) * 0.4f + 1.0f) * 0.5f;
}

void Game::initNoisePattern() {
    if (!noiseInitialized) {
        for (int ny = 0; ny < NOISE_SIZE; ny++) {
            for (int nx = 0; nx < NOISE_SIZE; nx++) {
                noisePatternArray[ny][nx] = smoothNoise(nx * PI, ny * E);
            }
        }
        noiseInitialized = true;
    }
}

void Game::initializeTextures() {
    if (!texturesInitialized) {
        // Initialize noise pattern first
        initNoisePattern();
        createAppleTexture();
        createStemAndLeafTexture();
        texturesInitialized = true;
    }
}

void Game::createAppleTexture() {
    const int textureSize = 64;
    SDL_Surface* surface = SDL_CreateRGBSurface(0, textureSize, textureSize, 32,
        0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    
    if (!surface) {
        std::cerr << "Failed to create surface: " << SDL_GetError() << std::endl;
        return;
    }

    const float centerX = textureSize / 2.0f;
    const float centerY = textureSize / 2.0f;
    const float radiusX = textureSize / 2.0f;
    const float radiusY = textureSize / 2.0f * 1.1f;
    
    SDL_LockSurface(surface);
    Uint32* pixels = static_cast<Uint32*>(surface->pixels);
    
    for (int y = 0; y < textureSize; y++) {
        for (int x = 0; x < textureSize; x++) {
            const float dx = (x - centerX);
            const float dy = (y - centerY);
            const float nx = dx / radiusX;
            const float ny = dy / radiusY;
            float distanceFromCenter = std::sqrt(nx * nx + ny * ny);
            
            if (dy < 0) {
                const float indentFactor = 0.12f;
                const float horizontalFactor = std::exp(-std::pow((dx / radiusX + 0.1f) * 2.5f, 2.0f));
                distanceFromCenter += indentFactor * (-ny) * horizontalFactor;
            }
            
            if (distanceFromCenter <= 1.0f) {
                const float angle = std::atan2(dy, dx);
                const int nx_int = static_cast<int>(std::cos(angle) * distanceFromCenter * NOISE_SCALE * NOISE_SIZE) & NOISE_MASK;
                const int ny_int = static_cast<int>(std::sin(angle) * distanceFromCenter * NOISE_SCALE * NOISE_SIZE) & NOISE_MASK;
                const float noise = noisePatternArray[ny_int][nx_int];
                
                const float radialGradient = 1.0f - distanceFromCenter * 0.3f;
                const float colorVar = (0.85f + noise * 0.3f) * radialGradient;
                const float baseColor = colorVar * currentGradient.colorVariation;
                
                const float dx_highlight = dx - radiusX * currentGradient.highlightX;
                const float dy_highlight = dy + radiusY * currentGradient.highlightY;
                const float highlightDistance = std::sqrt(dx_highlight * dx_highlight + dy_highlight * dy_highlight);
                const float highlightFactor = std::max(0.0f, 1.0f - (highlightDistance / (radiusX/1.5f)));
                const int highlight = static_cast<int>(80.0f * highlightFactor);
                
                Uint8 r = std::min(255, static_cast<int>(255.0f * baseColor) + highlight);
                Uint8 g = std::min(255, static_cast<int>(30.0f * baseColor) + highlight);
                Uint8 b = std::min(255, static_cast<int>(20.0f * baseColor) + highlight);
                Uint8 a = 255;
                
                pixels[y * surface->pitch/4 + x] = (a << 24) | (b << 16) | (g << 8) | r;
            } else {
                pixels[y * surface->pitch/4 + x] = 0;
            }
        }
    }
    
    SDL_UnlockSurface(surface);
    appleTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_SetTextureBlendMode(appleTexture, SDL_BLENDMODE_BLEND);
}

void Game::createStemAndLeafTexture() {
    const int textureSize = 64; // Increased to match apple size
    SDL_Surface* surface = SDL_CreateRGBSurface(0, textureSize, textureSize, 32,
        0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    
    SDL_LockSurface(surface);
    Uint32* pixels = static_cast<Uint32*>(surface->pixels);
    
    const float centerX = textureSize / 2.0f;
    const float stemWidth = textureSize * 0.3f;  // Increased width
    const float stemHeight = textureSize * 0.8f;
    
    for (int y = 0; y < textureSize; y++) {
        for (int x = 0; x < textureSize; x++) {
            // Render stem
            const float dx = (x - centerX);
            const float dy = y - (textureSize * 0.1f);  // Reduced offset
            const float stem_dist = std::abs(dx) / (stemWidth * 0.5f);
            
            if (stem_dist <= 1.0f && dy >= 0 && dy <= stemHeight) {
                const float alpha = 1.0f - std::pow(stem_dist, 2.0f);
                pixels[y * surface->pitch/4 + x] =
                    (101 << 24) |    // R (brown)
                    (67 << 16) |     // G
                    (33 << 8) |      // B
                    static_cast<Uint8>(255 * alpha);  // A
            }
            
            // Add leaf
            const float leaf_dx = dx - stemWidth * 0.3f;  // Adjusted leaf position
            const float leaf_dy = dy - stemHeight * 0.3f;
            const float leaf_dist = std::sqrt(leaf_dx*leaf_dx + leaf_dy*leaf_dy) / (stemWidth * 2.0f);
            
            if (leaf_dist <= 1.0f) {
                const float alpha = 1.0f - std::pow(leaf_dist, 2.0f);
                pixels[y * surface->pitch/4 + x] =
                    (34 << 24) |     // R (green)
                    (139 << 16) |    // G
                    (34 << 8) |      // B
                    static_cast<Uint8>(255 * alpha);  // A
            }
        }
    }
    
    SDL_UnlockSurface(surface);
    stemAndLeafTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_SetTextureBlendMode(stemAndLeafTexture, SDL_BLENDMODE_BLEND);
}

void Game::cleanupTextures() {
    if (appleTexture) {
        SDL_DestroyTexture(appleTexture);
        appleTexture = nullptr;
    }
    if (stemAndLeafTexture) {
        SDL_DestroyTexture(stemAndLeafTexture);
        stemAndLeafTexture = nullptr;
    }
    texturesInitialized = false;
}


// Snake Eyes
void Game::drawSnakeEyes(const SDL_Rect& headRect) {
    // Calculate eye positions based on snake's direction
    const int eyeSize = GRIDSIZE / 6;  // Size of each eye
    const int eyeOffset = GRIDSIZE / 4; // Distance from center
    
    // Base positions (center of the head)
    int centerX = headRect.x + headRect.w / 2;
    int centerY = headRect.y + headRect.h / 2;
    
    // Calculate eye positions based on direction
    int leftEyeX = centerX;
    int leftEyeY = centerY;
    int rightEyeX = centerX;
    int rightEyeY = centerY;
    
    switch(snakeBody[1].direction) {
        case Direction::UP:
            leftEyeX -= eyeOffset;
            rightEyeX += eyeOffset;
            leftEyeY = rightEyeY = centerY - eyeOffset;
            break;
        case Direction::DOWN:
            leftEyeX -= eyeOffset;
            rightEyeX += eyeOffset;
            leftEyeY = rightEyeY = centerY + eyeOffset;
            break;
        case Direction::LEFT:
            leftEyeX = rightEyeX = centerX - eyeOffset;
            leftEyeY -= eyeOffset;
            rightEyeY += eyeOffset;
            break;
        case Direction::RIGHT:
            leftEyeX = rightEyeX = centerX + eyeOffset;
            leftEyeY -= eyeOffset;
            rightEyeY += eyeOffset;
            break;
    }
    
    // Draw the white part of the eyes
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(int dy = -eyeSize; dy <= eyeSize; dy++) {
        for(int dx = -eyeSize; dx <= eyeSize; dx++) {
            if(dx*dx + dy*dy <= eyeSize*eyeSize) {
                SDL_RenderDrawPoint(renderer, leftEyeX + dx, leftEyeY + dy);
                SDL_RenderDrawPoint(renderer, rightEyeX + dx, rightEyeY + dy);
            }
        }
    }
    
    // Draw the pupils (black part of the eyes)
    const int pupilSize = eyeSize / 2;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for(int dy = -pupilSize; dy <= pupilSize; dy++) {
        for(int dx = -pupilSize; dx <= pupilSize; dx++) {
            if(dx*dx + dy*dy <= pupilSize*pupilSize) {
                SDL_RenderDrawPoint(renderer, leftEyeX + dx, leftEyeY + dy);
                SDL_RenderDrawPoint(renderer, rightEyeX + dx, rightEyeY + dy);
            }
        }
    }
}


// Update the Render function to use textures
void Game::Render() {
    if (gameOver) {
        displayLoseScreen();
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (snakeLength == 1) {
        addApple();
    }

    // Initialize textures if needed
    initializeTextures();

    // Render apple using texture
    SDL_Rect destRect = apple.apple;
    SDL_RenderCopy(renderer, appleTexture, NULL, &destRect);
    
    // Render stem and leaf
    SDL_Rect stemRect = {
        destRect.x + destRect.w/2 - destRect.w/8,
        destRect.y - destRect.h/4,
        destRect.w/4,
        destRect.h/2
    };
    SDL_RenderCopy(renderer, stemAndLeafTexture, NULL, &stemRect);

    // Render snake
    for (int i = 1; i < snakeLength; i++) {
        const float gradientPosition = static_cast<float>(i) / (snakeLength * gradientSteps);
        const int greenIntensity = std::max(100, 255 - static_cast<int>(gradientPosition * 155));
        const int blueIntensity = std::min(50, static_cast<int>(gradientPosition * 50));
        
        SDL_SetRenderDrawColor(renderer, 0, greenIntensity, blueIntensity, 255);
        squares[i] = { snakeBody[i].xPos, snakeBody[i].yPos, GRIDSIZE, GRIDSIZE };
        SDL_RenderFillRect(renderer, &squares[i]);
        
        SDL_SetRenderDrawColor(renderer, 0, greenIntensity, blueIntensity, 128);
        const SDL_Rect edgeRect = {
            snakeBody[i].xPos - 1,
            snakeBody[i].yPos - 1,
            GRIDSIZE + 2,
            GRIDSIZE + 2
        };
        SDL_RenderDrawRect(renderer, &edgeRect);
    }

    drawSnakeEyes(squares[1]);

    for (int i = snakeLength - 1; i > 1; i--) {
        drawSnakeConnector(i);
    }

    updateWindowTitle();
    SDL_RenderPresent(renderer);
}

