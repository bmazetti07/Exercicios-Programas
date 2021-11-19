import React, { useRef, useState, useEffect } from 'react';
import {
  SafeAreaView,
  View,
  Text,
  StyleSheet,
  Dimensions,
  Image,
  Animated,
  StatusBar
} from 'react-native';
import { TouchableOpacity } from 'react-native-gesture-handler';
import Ionicons from 'react-native-vector-icons/Ionicons';
import Entypo from 'react-native-vector-icons/Entypo';
import MaterialCommunityIcons from 'react-native-vector-icons/MaterialCommunityIcons';
import Slider from '@react-native-community/slider';
import songs from '../../model/data';

const { width } = Dimensions.get('window');

const MusicPlayer = ({ navigation }) => {
  const scrollX = useRef(new Animated.Value(0)).current;
  const [songIndex, setSongIndex] = useState(0);

  const songSlider = useRef(null);

  useEffect(() => {
    scrollX.addListener(({ value }) => {
      const index = Math.round(value / width);
      setSongIndex(index);
    })

    return () => {
      scrollX.removeAllListeners();
    }
  }, []);

  const skipToNext = () => {
    songSlider.current.scrollToOffset({
      offset: (songIndex + 1) * width,
    });
  }

  const skipToPrevious = () => {
    songSlider.current.scrollToOffset({
      offset: (songIndex - 1) * width,
    });
  }

  const renderSongs = ({ index, item }) => {
    return (
      <Animated.View style={{
        width: width,
        justifyContent: 'center',
        alignItems: 'center'
      }}>
        <View style={styles.artworkWrapper}>
          <Image
            source={item.image}
            style={styles.artworkImage}
          />
        </View>
      </Animated.View>
    );
  };

  return (
    <SafeAreaView style={styles.container}>
      <StatusBar barStyle='default' />
      <View style={styles.mainContainer}>
        <View style={{ width: width }}>
          <Animated.FlatList
            ref={songSlider}
            data={songs}
            renderItem={renderSongs}
            keyExtractor={(item) => item.id}
            horizontal
            pagingEnabled
            showsHorizontalScrollIndicator={false}
            scrollEventThrottle={16}
            onScroll={Animated.event(
              [{
                nativeEvent: {
                  contentOffset: { x: scrollX }
                }
              }],
              { useNativeDriver: true }
            )}
          />
        </View>

        <View>
          <Text style={styles.title}> {songs[songIndex].title} </Text>
          <Text style={styles.artist}> {songs[songIndex].artist} </Text>
        </View>

        <View>
          <Slider
            style={styles.progressContainer}
            value={10}
            minimumValue={0}
            maximumValue={100}
            thumbTintColor='#FFD369'
            minimumTrackTintColor='#FFD369'
            maximumTrackTintColor='#FFF'
            onSlidingComplete={() => { }}
          />

          <View style={styles.progressLabelContainer}>
            <Text style={styles.progressLabelTxt}> 0:00 </Text>
            <Text style={styles.progressLabelTxt}> 4:00 </Text>
          </View>
        </View>

        <View style={styles.musicControls}>
          <TouchableOpacity onPress={skipToPrevious}>
            <Ionicons name="play-skip-back-outline" size={35} color={'#FFD369'} style={{ marginTop: 25 }} />
          </TouchableOpacity>
          <TouchableOpacity onPress={() => { }}>
            <Ionicons name="ios-pause-circle" size={75} color={'#FFD369'} />
          </TouchableOpacity>
          <TouchableOpacity onPress={skipToNext}>
            <Ionicons name="play-skip-forward-outline" size={35} color={'#FFD369'} style={{ marginTop: 25 }} />
          </TouchableOpacity>
        </View>
      </View>

      <View style={styles.bottomContainer}>
        <View style={styles.bottomControls}>
          <TouchableOpacity onPress={() => navigation.navigate('PlayingScreen')}>
            <MaterialCommunityIcons color='#fff' name="headphones" size={30} />
          </TouchableOpacity>

          <TouchableOpacity onPress={() => navigation.navigate('SongsList')}>
            <Entypo color='#fff' name="list" size={30} />
          </TouchableOpacity>

          <TouchableOpacity onPress={() => navigation.navigate('LoadingScreen')}>
            <MaterialCommunityIcons color='#fff' name="cloud" size={30} />
          </TouchableOpacity>
        </View>
      </View>
    </SafeAreaView>
  );
};

export default MusicPlayer;

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#222831'
  },

  mainContainer: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center'
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
  },

  artworkWrapper: {
    width: 300,
    height: 340,
    marginBottom: 25,
    elevation: 5
  },

  artworkImage: {
    width: '100%',
    height: '100%',
    borderRadius: 15
  },

  title: {
    fontSize: 18,
    fontWeight: '600',
    textAlign: 'center',
    color: '#EEEEEE'
  },

  artist: {
    fontSize: 16,
    fontWeight: '200',
    textAlign: 'center',
    color: '#EEEEEE'
  },

  progressContainer: {
    width: 350,
    height: 40,
    marginTop: 25,
    flexDirection: 'row'
  },

  progressLabelContainer: {
    width: 340,
    flexDirection: 'row',
    justifyContent: 'space-between'
  },

  progressLabelTxt: {
    color: '#fff'
  },

  musicControls: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    width: '60%',
    marginTop: 15
  }
});