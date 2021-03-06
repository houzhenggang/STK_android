#ifndef HEADER_FEATURE_UNLOCKED_HPP
#define HEADER_FEATURE_UNLOCKED_HPP

#include "graphics/irr_driver.hpp"
#include "guiengine/screen.hpp"
#include "utils/ptr_vector.hpp"

namespace irr { namespace scene { class ISceneNode; class ICameraSceneNode; class ILightSceneNode; } }
class KartProperties;
class Challenge;

/**
  * \brief Screen shown when a feature has been unlocked
  * \ingroup states_screens
 */
class FeatureUnlockedCutScene : public GUIEngine::Screen, public GUIEngine::ScreenSingleton<FeatureUnlockedCutScene>
{
    friend class GUIEngine::ScreenSingleton<FeatureUnlockedCutScene>;
    
    FeatureUnlockedCutScene();
    
    /** Whichever of these is non-null decides what comes out of the chest */
    struct UnlockedThing
    {
        /** Will be non-null if this unlocked thing is a kart */
        KartProperties* m_unlocked_kart;
        
        /** Will be non-empty if this unlocked thing is one or many pictures */
        std::vector<irr::video::ITexture*> m_pictures;
        /** Will be set if this unlocked thing is a picture */
        float m_w, m_h;
        /** used for slideshows */
        int m_curr_image;
        
        /** Contains whatever is in the chest */
        scene::ISceneNode* m_root_gift_node;
        
        irr::core::stringw m_unlock_message;
        
        UnlockedThing(KartProperties* kart, irr::core::stringw msg);
        
        /**
          * Creates a 'picture' reward.
          * \param pict  the picture to display as reward.
          * \param w     width of the picture to display
          * \param y     height of the picture to display
          */
        UnlockedThing(irr::video::ITexture* pict, float w, float h, irr::core::stringw msg);
        
        /**
         * Creates a 'picture slideshow' reward.
         * \param picts the pictures to display as reward.
         * \param w     width of the pictures to display
         * \param y     height of the pictures to display
         */
        UnlockedThing(std::vector<irr::video::ITexture*> picts, float w, float h, irr::core::stringw msg);
        
        ~UnlockedThing();
    };

    /** The list of all unlocked things. */
    ptr_vector<UnlockedThing, HOLD> m_unlocked_stuff;
    
    /** To store the copy of the KartModel for each unlocked kart. */
    std::vector<KartModel*> m_all_kart_models;
    /** sky angle, 0-360 */
    float m_sky_angle;
    
    /** Global evolution of time */
    double m_global_time;
    
    /** Key position from origin (where the chest is) */
    float m_key_pos;
    
    /** Angle of the key (from 0 to 1, simply traces progression) */
    float m_key_angle;
    
    /** The scene node for the sky box. */
    irr::scene::ISceneNode             *m_sky;

    /** The scene node for the camera. */
    irr::scene::ICameraSceneNode       *m_camera;

    /** The scene node for the animated mesh. */
    irr::scene::IAnimatedMeshSceneNode *m_chest;

    /** The scene node for the light. */
    irr::scene::ILightSceneNode* m_light;
    
    //#define USE_IRRLICHT_BUG_WORKAROUND
    
#ifdef USE_IRRLICHT_BUG_WORKAROUND
    scene::IMeshSceneNode *m_avoid_irrlicht_bug;
#endif
    
    void continueButtonPressed();
    
public:

    /** \brief implement optional callback from parent class GUIEngine::Screen */
    void onUpdate(float dt, irr::video::IVideoDriver*);
    
    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void loadedFromFile();
    
    /** \brief implement callback from parent class GUIEngine::Screen */
    void init();
    
    /** \brief implement callback from parent class GUIEngine::Screen */
    void tearDown();
    
    void eventCallback(GUIEngine::Widget* widget, const std::string& name, const int playerID);
    
    /** Call before showing up the screen to make a kart come out of the chest.
        'addUnlockedThings' will invoke this, so you generally don't need to call this directly. */
    void addUnlockedKart(KartProperties* unlocked_kart, irr::core::stringw msg);
    
    /** Call before showing up the screen to make a picture come out of the chest
        'addUnlockedThings' will invoke this, so you generally don't need to call this directly. */
    void addUnlockedPicture(irr::video::ITexture* picture, float w, float h, irr::core::stringw msg);
    
    /** Call before showing up the screen to make a picture slideshow come out of the chest
        'addUnlockedThings' will invoke this, so you generally don't need to call this directly. */
    void addUnlockedPictures(std::vector<irr::video::ITexture*> pictures,
                             float w, float h, irr::core::stringw msg);
    
    /** Call before showing up the screen to make whatever the passed challenges unlocked
      * come out of the chest */
    void addUnlockedThings(const std::vector<const Challenge*> unlocked);
    
    
    /** override from base class to handle escape press */
    virtual bool onEscapePressed();
};

#endif

