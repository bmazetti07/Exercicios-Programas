import React, { Component } from 'react';
import {
  SafeAreaView,
  View,
  StyleSheet,
  Dimensions,
  StatusBar,
} from 'react-native';
import { TouchableOpacity } from 'react-native-gesture-handler';
import { AudioContext } from '../context/AudioProvider';
import Entypo from 'react-native-vector-icons/Entypo';
import MaterialCommunityIcons from 'react-native-vector-icons/MaterialCommunityIcons';
import { RecyclerListView, LayoutProvider } from 'recyclerlistview';
import AudioListItem from '../components/AudioListItem';
import OptionModal from '../components/OptionModal';
import { Audio } from 'expo-av'
import { play, pause, resume } from '../misc/audioController'

const { width, height } = Dimensions.get('window');

export class SongsList extends Component {
  static contextType = AudioContext

  constructor(props) {
    super(props)
    this.state = {
      optionModalVisible: false
    }

    this.currentItem = {

    }
  }

  layoutProvider = new LayoutProvider((i) => 'audio', (type, dim) => {
    switch (type) {
      case 'audio':
        dim.width = Dimensions.get('window').width
        dim.height = 70
        break
      default:
        dim.width = 0
        dim.height = 0
    }
  })

  handleAudioPress = async (audio) => {
    const { soundObj, playbackObj, currentAudio, updateState } = this.context
    if (soundObj === null) {
      const playbackObj = new Audio.Sound()
      const status = await play(playbackObj, audio.uri)

      return updateState(this.context,
        {
          currentAudio: audio,
          playbackObj: playbackObj,
          soundObj: status
        }
      )
    }

    if (soundObj.isLoaded && soundObj.isPlaying) {
      const status = await pause(playbackObj)

      return updateState(this.context, { soundObj: status })
    }

    if (soundObj.isLoaded && !soundObj.isPlaying && currentAudio.id === audio.id) {
      const status = await resume(playbackObj)

      return updateState(this.context, { soundObj: status })
    }
  }

  rowRenderer = (type, item) => {
    return (
      <AudioListItem
        title={item.filename}
        duration={item.duration}
        onAudioPress={() => this.handleAudioPress(item)}
        on onOptionPress={() => {
          this.currentItem = item
          this.setState({ ...this.state, optionModalVisible: true })
        }}>
      </AudioListItem>
    )
  }

  render() {
    return (
      <SafeAreaView style={styles.container}>
        <StatusBar barStyle='default' />

        <AudioContext.Consumer>
          {({ dataProvider }) => {
            return (
              <View style={{ flex: 1 }}>
                <RecyclerListView
                  dataProvider={dataProvider}
                  layoutProvider={this.layoutProvider}
                  rowRenderer={this.rowRenderer}
                />

                <OptionModal
                  onPlayPress={() => console.log('Tocando')}
                  currentItem={this.currentItem}
                  onClose={() => this.setState({ ...this.state, optionModalVisible: false })}
                  visible={this.state.optionModalVisible}
                />

              </View>
            )
          }}
        </AudioContext.Consumer>

        <View style={styles.bottomContainer}>
          <View style={styles.bottomControls}>
            <TouchableOpacity onPress={() => this.props.navigation.navigate('PlayingScreen')}>
              <MaterialCommunityIcons color='#fff' name="headphones" size={30} />
            </TouchableOpacity>

            <TouchableOpacity onPress={() => this.props.navigation.navigate('SongsList')}>
              <Entypo color='#fff' name="list" size={30} />
            </TouchableOpacity>

            <TouchableOpacity onPress={() => this.props.navigation.navigate('LoadingScreen')}>
              <MaterialCommunityIcons color='#fff' name="cloud" size={30} />
            </TouchableOpacity>
          </View>
        </View>
      </SafeAreaView >
    );
  };
};

export default SongsList;

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#222831'
  },

  bottomContainer: {
    borderTopColor: '#393E46',
    borderTopWidth: 1,
    width: width,
    alignItems: 'center',
    paddingVertical: 15
  },

  bottomControls: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    width: '80%'
  }
})