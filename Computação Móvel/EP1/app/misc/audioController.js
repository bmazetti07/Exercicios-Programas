// Tocar áudio
export const play = async (playbackObj, uri) => {
  try {
    return await playbackObj.loadAsync(
      { uri },
      { shouldPlay: true }
    )
  } catch (error) {
    console.log('Erro ao tocar', error.message)
  }
}

// Pausar Áudio
export const pause = async (playbackObj) => {
  try {
    return await playbackObj.setStatusAsync(
      { shouldPlay: false }
    )
  } catch (error) {
    console.log('Erro ao pausar', error.message)
  }
}

// Continuar o Áudio
export const resume = async (playbackObj) => {
  try {
    return await playbackObj.playAsync()
  } catch (error) {
    console.log('Erro ao continuar a música', error.message)
  }
}