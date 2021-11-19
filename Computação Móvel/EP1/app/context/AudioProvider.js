import React, { Component, createContext } from 'react';
import { Text, View, Alert, StyleSheet } from 'react-native';
import * as MediaLibrary from 'expo-media-library';
import MaterialIcons from 'react-native-vector-icons/MaterialIcons';
import { DataProvider } from 'recyclerlistview';

export const AudioContext = createContext()
export class AudioProvider extends Component {
  constructor(props) {
    super(props)
    this.state = {
      audioFiles: [],
      permissionError: false,
      dataProvider: new DataProvider((r1, r2) => r1 !== r2),
      playbackObj: null,
      soundObj: null,
      currentAudio: {}
    }
  }

  permissionAlert = () => {
    Alert.alert("É necessário permissão!", "Esse aplicativo precisa ler arquivos de áudio!", [{
      text: 'Garantir Permissão',
      onPress: () => this.getPermission()
    }, {
      text: 'Negar Permissão',
      onPress: () => this.permissionAlert()
    }])
  }

  getAudioFiles = async () => {
    const { dataProvider, audioFiles } = this.state
    let media = await MediaLibrary.getAssetsAsync({
      mediaType: 'audio',
    })

    media = await MediaLibrary.getAssetsAsync({
      mediaType: 'audio',
      first: media.totalCount,
      sortBy: MediaLibrary.SortBy.modificationTime
    })

    this.setState({ ...this.state, dataProvider: dataProvider.cloneWithRows([...audioFiles, ...media.assets]), audioFiles: [...audioFiles, ...media.assets] })
  }

  getPermission = async () => {
    const permission = await MediaLibrary.getPermissionsAsync()

    if (permission.granted) {
      this.getAudioFiles()
    }

    if (!permission.canAskAgain && !permission.granted) {
      this.setState({ ...this.state, permissionError: true })
    }

    if (!permission.granted && permission.canAskAgain) {
      const { status, canAskAgain } = await MediaLibrary.requestPermissionsAsync()
      if (status === 'denied' && canAskAgain) {
        this.permissionAlert()
      }

      if (status === 'granted') {
        this.getAudioFiles()
      }

      if (status === 'denied' && !canAskAgain) {
        this.setState({ ...this.state, permissionError: true })
      }
    }
  }

  componentDidMount() {
    this.getPermission()
  }

  updateState = (prevState, newState = {}) => {
    this.setState({ ...prevState, ...newState })
  }

  render() {
    const { audioFiles, dataProvider, permissionError, playbackObj, soundObj, currentAudio } = this.state

    if (permissionError)
      return (
        <View style={styles.container}>
          <MaterialIcons name="error" size={100} color={'red'} style={{ marginTop: 25 }} />
          <Text style={styles.permissionDeniedText}> O aplicativo não executará, pois não há permissão de leitura de mídia </Text>
        </View>
      )

    return (
      <AudioContext.Provider value={{
        audioFiles,
        dataProvider,
        playbackObj,
        soundObj,
        currentAudio,
        updateState: this.updateState
      }}>
        {this.props.children}
      </AudioContext.Provider>
    )
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#222831'
  },

  permissionDeniedText: {
    fontSize: 25,
    textAlign: 'center',
    color: '#fff'
  }
})

export default AudioProvider
